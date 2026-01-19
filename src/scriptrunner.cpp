#include "scriptrunner.h"

ScriptRunner::ScriptRunner(Script& script, AudioManager& audio_manager, sdl::Renderer& renderer)
	: current_script_index_(0), previous_script_index_(current_script_index_), script_index_when_prev_({false, current_script_index_}), script_(script), init_(false),
	character_manager_(renderer, script.character_definitions_), background_manager_(renderer), music_manager_(audio_manager), sound_manager_(audio_manager),
	textbox_manager_(renderer), dialogue_manager_(renderer)
{
	character_manager_.create_narrator();
}

void ScriptRunner::increment_script_index()
{
	if(script_.script_information_.size() > current_script_index_ + 1)
	{
		current_script_index_ += 1;
	}
}

void ScriptRunner::decrement_script_index()
{
	if(current_script_index_ > 0)
	{
		current_script_index_ -= 1;
	}
}

bool ScriptRunner::is_current_script_index_a_dialogue()
{
	return std::holds_alternative<Script::InfoDialogue>(script_.script_information_[current_script_index_]);
}

bool ScriptRunner::move_dialogue(DialogueManager::Where where, bool is_from_mouse_wheel_)
{
	//std::cout << "AVANT: " << current_script_index_ << ", " << previous_script_index_ << std::endl;

	if(where == DialogueManager::Where::next)
	{
		if(is_from_mouse_wheel_ && current_script_index_ < script_index_when_prev_.saved_script_index_)
		{
			increment_script_index();
		}
		else if(!is_from_mouse_wheel_)
		{
			script_index_when_prev_.is_saved_ = false; //when we pass a dialogue, reset the mouse wheel dialogues
			increment_script_index();
		}
	}
	else if(where == DialogueManager::Where::prev)
	{
		if(!script_index_when_prev_.is_saved_) //sauvegarder une seule fois 
		{
			script_index_when_prev_.saved_script_index_ = current_script_index_;
			script_index_when_prev_.is_saved_ = true;
		}

		decrement_script_index();

		if(current_script_index_ == 0 && !is_current_script_index_a_dialogue())
		{
			current_script_index_ = previous_script_index_; //on est allé en arrière sans croiser de dialogue => revenir au tout premier dialogue
		}
	}
	//std::cout << "APRES: " << current_script_index_ << ", " << previous_script_index_ << std::endl;

	bool is_dialogue = is_current_script_index_a_dialogue();
	if(is_dialogue)
	{
		previous_script_index_ = current_script_index_;
	}
	return is_dialogue;
}

void ScriptRunner::handle_events(const SDL_Event& e)
{
	dialogue_manager_.handle_events_mouse_wheel(e);
	dialogue_manager_.handle_events_keyboard_mouse(e);

	if(e.type == AudioManager::END_MUSIC_EVENT_)
	{
		if(music_manager_.music_ != nullptr)
		{
			music_manager_.fade_in(*music_manager_.music_, music_manager_.audio_properties_);
		}
	}
	else if(e.type == AudioManager::END_CHANNEL_EVENT_)
	{
		if(sound_manager_.sound_ != nullptr && !sound_manager_.played_)
		{
			sound_manager_.fade_in(*sound_manager_.sound_, sound_manager_.audio_properties_);
		}
	}
}

void ScriptRunner::draw(sdl::Renderer& renderer)
{
	background_manager_.draw(renderer);
	character_manager_.draw(renderer);
	dialogue_manager_.draw(renderer);
}

//TODO : utiliser std::visit ??
void ScriptRunner::apply_line(size_t script_index)
{
	Script::ScriptInformation& current_script_information = script_.script_information_[script_index];
	if(std::holds_alternative<Script::InfoBackground>(current_script_information))
	{
		background_manager_.update(std::get<Script::InfoBackground>(current_script_information));
	}
	else if(std::holds_alternative<Script::InfoCharacter>(current_script_information))
	{
		character_manager_.update(std::get<Script::InfoCharacter>(current_script_information));
	}
	else if(std::holds_alternative<Script::InfoAutofocus>(current_script_information))
	{
		//update_autofocus(std::get<Script::InfoAutofocus>(current_script_information));
	}
	else if(std::holds_alternative<Script::InfoMusic>(current_script_information))
	{
		music_manager_.update(std::get<Script::InfoMusic>(current_script_information));
	}
	else if(std::holds_alternative<Script::InfoSound>(current_script_information))
	{
		sound_manager_.update(std::get<Script::InfoSound>(current_script_information));
	}
	else if(std::holds_alternative<Script::InfoDialogue>(current_script_information))
	{
		Script::InfoDialogue& info_dialogue = std::get<Script::InfoDialogue>(current_script_information);
		dialogue_manager_.update(info_dialogue, character_manager_.active_characters_.at(info_dialogue.character_variable_));
		//update_characters_dialogue(info_dialogue);
		dialogue_manager_.which_dialogue_from_where_ = {DialogueManager::Where::none, false, false};
	}
	else if(std::holds_alternative<Script::InfoTextbox>(current_script_information))
	{
		//update_textbox(std::get<Script::InfoTextbox>(current_script_information));
	}
}

void ScriptRunner::update()
{
	//std::cout << "CURRENT: " << current_script_index_ << std::endl;

	if(dialogue_manager_.which_dialogue_from_where_.which_dialogue_ == DialogueManager::Where::prev)
	{
		rebuild();
	}
	else
	{
		move_dialogue(dialogue_manager_.which_dialogue_from_where_.which_dialogue_, dialogue_manager_.which_dialogue_from_where_.is_from_mouse_wheel_);
	}

	if(!init_)
	{
		if(!std::holds_alternative<Script::InfoDialogue>(script_.script_information_[current_script_index_]))
		{
			dialogue_manager_.which_dialogue_from_where_ = {DialogueManager::Where::next, false, false};
		}
		else
		{
			init_ = true;
		}
	}

	apply_line(current_script_index_);
	
	//TODO : pas ouf placé ici...
	for(auto& [key_character_variable, value_character] : character_manager_.active_characters_)
	{
		value_character.update();
	}
}

void ScriptRunner::play_all_sounds_before_previous_dialogue(size_t target_script_index)
{
	for(size_t i = target_script_index; i-- > 0; )
	{
		if(std::holds_alternative<Script::InfoDialogue>(script_.script_information_[i]))
		{
			break;
		}
		else if(std::holds_alternative<Script::InfoSound>(script_.script_information_[i]))
		{
			sound_manager_.update(std::get<Script::InfoSound>(script_.script_information_[i]));
		}
	}
}

void ScriptRunner::handle_music_when_scroll_back(size_t target_script_index)
{
	bool music_found = false;
	for(size_t i = target_script_index; i-- > 0; )
	{
		if(std::holds_alternative<Script::InfoMusic>(script_.script_information_[i]))
		{
			music_manager_.update(std::get<Script::InfoMusic>(script_.script_information_[i]));
			music_found = true;
			break;
		}
	}

	if(!music_found)
	{
		music_manager_.stop(constants::fadeout_length_scroll_back_); //si scroll en arrière et aucune musique => fadeout de 1.5 secondes (valeur constante)
	}
}

void ScriptRunner::rebuild()
{
	size_t target_script_index = current_script_index_;

	character_manager_.reset();
	dialogue_manager_.reset();
	sound_manager_.reset();

	do
	{
		if(target_script_index > 0)
		{
			target_script_index -= 1;
		}
	} while(!std::holds_alternative<Script::InfoDialogue>(script_.script_information_[target_script_index]));

	//std::cout << "target script index: " << target_script_index << ", current script index: " << current_script_index_ << std::endl;

	for(size_t i = 0; i <= target_script_index; ++i)
	{
		Script::ScriptInformation& current_script_information = script_.script_information_[i];
		if(std::holds_alternative<Script::InfoBackground>(current_script_information))
		{
			background_manager_.update(std::get<Script::InfoBackground>(current_script_information));
		}
		else if(std::holds_alternative<Script::InfoCharacter>(current_script_information))
		{
			character_manager_.update(std::get<Script::InfoCharacter>(current_script_information));
		}
		else if(std::holds_alternative<Script::InfoAutofocus>(current_script_information))
		{
			//update_autofocus(std::get<Script::InfoAutofocus>(current_script_information));
		}
		//else if(std::holds_alternative<Script::InfoMusic>(current_script_information))
		//{
		//	//music_manager_.update(std::get<Script::InfoMusic>(current_script_information));
		//}
		//else if(std::holds_alternative<Script::InfoSound>(current_script_information))
		//{
		//	//update_sounds(std::get<Script::InfoSound>(current_script_information), current_script_index_);
		//	sound_manager_.update(std::get<Script::InfoSound>(current_script_information), script_index);
		//}
		else if(std::holds_alternative<Script::InfoDialogue>(current_script_information))
		{
			Script::InfoDialogue& info_dialogue = std::get<Script::InfoDialogue>(current_script_information);
			dialogue_manager_.update(info_dialogue, character_manager_.active_characters_.at(info_dialogue.character_variable_));
			//update_characters_dialogue(info_dialogue);
		}
		else if(std::holds_alternative<Script::InfoTextbox>(current_script_information))
		{
			//update_textbox(std::get<Script::InfoTextbox>(current_script_information));
		}
		move_dialogue(dialogue_manager_.which_dialogue_from_where_.which_dialogue_, dialogue_manager_.which_dialogue_from_where_.is_from_mouse_wheel_);
	}
	play_all_sounds_before_previous_dialogue(target_script_index);
	handle_music_when_scroll_back(target_script_index);
	
	dialogue_manager_.which_dialogue_from_where_ = {DialogueManager::Where::none, false, false};
	current_script_index_ = target_script_index;
}