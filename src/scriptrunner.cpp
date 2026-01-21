#include "scriptrunner.h"

//TODO : le code des dialogues devra être modifié quand il y aura l'ajout de pauses, animations d'images, choice menus etc.

ScriptRunner::ScriptRunner(Game& game, sdl::Renderer& renderer)
	: current_script_index_(0), script_index_when_prev_({false, current_script_index_}), script_(game.script_), init_(false),
	character_manager_(renderer, game.script_.character_definitions_), background_manager_(renderer), music_manager_(game.audio_manager_), sound_manager_(game.audio_manager_), textbox_manager_(renderer, game)
{
	character_manager_.create_narrator();
	init_dialogues_script_index();
}

void ScriptRunner::init_dialogues_script_index()
{
	for(size_t i = 0; i < script_.script_information_.size(); ++i)
	{
		if(std::holds_alternative<Script::InfoTextbox>(script_.script_information_[i]) && std::get<Script::InfoTextbox>(script_.script_information_[i]).t_.textbox_command_kind_ == Script::TextboxCommandKind::DIALOGUE)
		{
			dialogues_script_index_.insert(i);
		}
	}
}

std::optional<size_t> ScriptRunner::get_script_index_of_previous_dialogue()
{
	std::set<size_t>::iterator it = dialogues_script_index_.lower_bound(current_script_index_);
	if(it != dialogues_script_index_.begin() && it != dialogues_script_index_.end())
	{
		it = std::prev(it);
		return *it;
	}
	return std::nullopt;
}

size_t ScriptRunner::get_script_index_of_first_dialogue()
{
	return *(dialogues_script_index_.begin());
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
	return dialogues_script_index_.count(current_script_index_);
}

bool ScriptRunner::is_script_index_a_dialogue(size_t script_index)
{
	return dialogues_script_index_.count(script_index);
}

void ScriptRunner::save_current_script_index_when_scroll_back()
{
	if(!script_index_when_prev_.is_saved_) //sauvegarder une seule fois 
	{
		script_index_when_prev_.saved_script_index_ = current_script_index_;
		script_index_when_prev_.is_saved_ = true;
	}
}

//TODO : mettre dans TextboxManager ??
void ScriptRunner::move_dialogue(TextboxManager::Where where, bool is_from_mouse_wheel_)
{
	//std::cout << "AVANT: " << current_script_index_ << ", " << std::boolalpha << (where == TextboxManager::Where::none) << std::endl;
	if(where == TextboxManager::Where::next)
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
	else if(where == TextboxManager::Where::prev)
	{
		save_current_script_index_when_scroll_back();
		decrement_script_index();

		if(current_script_index_ == 0 && !is_current_script_index_a_dialogue())
		{
			current_script_index_ = get_script_index_of_first_dialogue(); //on est allé en arrière sans croiser de dialogue => revenir au tout premier dialogue
		}
	}
	//std::cout << "APRES: " << current_script_index_ << std::endl;
}

void ScriptRunner::handle_events(const SDL_Event& e)
{
	textbox_manager_.handle_events(e);

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
	textbox_manager_.draw(renderer);
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
		character_manager_.update_autofocus(std::get<Script::InfoAutofocus>(current_script_information));
	}
	else if(std::holds_alternative<Script::InfoMusic>(current_script_information))
	{
		music_manager_.update(std::get<Script::InfoMusic>(current_script_information));
	}
	else if(std::holds_alternative<Script::InfoSound>(current_script_information))
	{
		sound_manager_.update(std::get<Script::InfoSound>(current_script_information));
	}
	else if(std::holds_alternative<Script::InfoTextbox>(current_script_information))
	{
		Script::InfoTextbox& info_textbox = std::get<Script::InfoTextbox>(current_script_information);
		textbox_manager_.update(info_textbox, character_manager_.active_characters_.at(info_textbox.character_variable_));
		character_manager_.update_characters_dialogue(info_textbox);
		//textbox_manager_.which_dialogue_from_where_ = {TextboxManager::Where::none, false, false};
	}
}

void ScriptRunner::update()
{
	std::cout << "CURRENT: " << current_script_index_ << std::endl;
	//std::cout << script_index_when_prev_.saved_script_index_ << std::endl;
	//std::cout << std::boolalpha << autofocus_manager_.autofocus_ << std::endl;

	if(textbox_manager_.which_dialogue_from_where_.which_dialogue_ == TextboxManager::Where::prev)
	{
		rebuild();
	}

	textbox_manager_.update_skip_auto_modes(); //car doit être exécuté avant move_dialogue()
	move_dialogue(textbox_manager_.which_dialogue_from_where_.which_dialogue_, textbox_manager_.which_dialogue_from_where_.is_from_mouse_wheel_);

	if(!init_)
	{
		//std::cout << current_script_index_ << " et " << std::boolalpha << (std::holds_alternative<Script::InfoCharacter>(script_.script_information_[current_script_index_])) << std::endl
		
		if(!is_current_script_index_a_dialogue())
		{
			textbox_manager_.which_dialogue_from_where_ = {TextboxManager::Where::next, false, false};
		}
		else
		{
			textbox_manager_.which_dialogue_from_where_ = {TextboxManager::Where::none, false, false}; //TODO : utile ??
			init_ = true;
		}
	}

	apply_line(current_script_index_);
	//move_dialogue(textbox_manager_.which_dialogue_from_where_.which_dialogue_, textbox_manager_.which_dialogue_from_where_.is_from_mouse_wheel_);
	
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
		if(is_script_index_a_dialogue(i))
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
	if(!get_script_index_of_previous_dialogue().has_value()) //cas où on tente d'aller à un dialogue précédent alors qu'on est déjà sur le premier dialogue
	{
		return;
	}

	character_manager_.reset();
	textbox_manager_.reset();
	sound_manager_.reset();
	background_manager_.reset();

	size_t target_script_index = get_script_index_of_previous_dialogue().value();
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
			character_manager_.update_autofocus(std::get<Script::InfoAutofocus>(current_script_information));
		}
		//else if(std::holds_alternative<Script::InfoMusic>(current_script_information))
		//{
		//	//music_manager_.update(std::get<Script::InfoMusic>(current_script_information));
		//}
		//else if(std::holds_alternative<Script::InfoSound>(current_script_information))
		//{
		//	//update_sounds(std::get<Script::InfoSound>(current_script_information), current_script_index_);
		//	sound_manager_.update(std::get<Script::InfoSound>(current_script_information));
		//}
		else if(std::holds_alternative<Script::InfoTextbox>(current_script_information)) 
		{
			Script::InfoTextbox& info_textbox = std::get<Script::InfoTextbox>(current_script_information);
			if(info_textbox.t_.textbox_command_kind_ == Script::TextboxCommandKind::MOVE_TEXTBOX) //TODO : pas ouf...
			{
				textbox_manager_.update(info_textbox, character_manager_.active_characters_.at(info_textbox.character_variable_));
			}
			character_manager_.update_characters_dialogue(info_textbox);
		}
	}
	save_current_script_index_when_scroll_back();
	play_all_sounds_before_previous_dialogue(target_script_index);
	handle_music_when_scroll_back(target_script_index);
	
	textbox_manager_.which_dialogue_from_where_ = {TextboxManager::Where::none, false, false};
	current_script_index_ = target_script_index;
}