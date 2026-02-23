#include "script_runner.h"

#include <iostream>

//TODO : le code des dialogues devra être modifié quand il y aura l'ajout de pauses, animations d'images, choice menus etc.

ScriptRunner::ScriptRunner(Game& game, sdl::Renderer& renderer)
	: current_script_index_(0), script_index_when_prev_({false, current_script_index_}), script_(game.script_), init_(false), is_choice_menu_visible_(false), is_dialogue_of_choice_menu_visible_(false),
	character_manager_(renderer, game.script_.character_definitions_), background_manager_(renderer), music_manager_(game.audio_manager_), sound_manager_(game.audio_manager_), textbox_manager_(renderer, game), choice_menu_manager_(renderer, game)
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
	return *(dialogues_script_index_.begin()); //TODO : pourquoi un itérateur => [0] fonctionnerait ?
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

void ScriptRunner::move_dialogue(TextboxManager::Where where, bool is_from_mouse_wheel_)
{
	//std::cout << "AVANT: " << current_script_index_ << ", " << std::boolalpha << (where == TextboxManager::Where::none) << std::endl;
	if(where == TextboxManager::Where::NEXT)
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
	else if(where == TextboxManager::Where::PREV)
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

void ScriptRunner::init_to_first_dialogue()
{
	if(!init_)
	{
		if(!is_current_script_index_a_dialogue())
		{
			textbox_manager_.dialogue_instruction_ = {TextboxManager::Where::NEXT, false, false};
		}
		else
		{
			textbox_manager_.dialogue_instruction_ = {TextboxManager::Where::NONE, false, false}; //TODO : utile ??
			init_ = true;
		}
	}
}

void ScriptRunner::handle_events(const SDL_Event& e)
{
	if(is_dialogue_of_choice_menu_visible_)
	{
		textbox_manager_.ui_manager_.handle_events(e); //TODO : ne pas accéder à ui_manager_ directement => créer une méthode
		textbox_manager_.handle_events_mouse_wheel(e); //TODO : doit être là
	}
	else
	{
		textbox_manager_.handle_events(e);
	}

	transition_manager_.handle_events(e);
	choice_menu_manager_.handle_events(e);

	if(e.type == AudioManager::END_MUSIC_EVENT_)
	{
		if(music_manager_.music_ != nullptr)
		{
			music_manager_.fade_in(*music_manager_.music_, music_manager_.audio_properties_); //TODO : les deux paramètres sont-ils nécessaires car ce sont des membres ?
		}
	}
	else if(e.type == AudioManager::END_CHANNEL_EVENT_)
	{
		if(sound_manager_.sound_ != nullptr && !sound_manager_.played_)
		{
			sound_manager_.fade_in(*sound_manager_.sound_, sound_manager_.audio_properties_); //TODO : les deux paramètres sont-ils nécessaires car ce sont des membres ?
		}
	}
}

void ScriptRunner::draw(sdl::Renderer& renderer)
{
	background_manager_.draw(renderer);
	character_manager_.draw(renderer);
	choice_menu_manager_.draw(renderer);

	if(!transition_manager_.transition_playing_)
	{
		textbox_manager_.draw(renderer);
	}

	transition_manager_.draw(renderer);
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
		if(is_choice_menu_visible_)
		{
			is_dialogue_of_choice_menu_visible_ = true;
			textbox_manager_.uncheck_auto_toggle();
			textbox_manager_.uncheck_skip_toggle();
		}

		Script::InfoTextbox& info_textbox = std::get<Script::InfoTextbox>(current_script_information);
		textbox_manager_.update(info_textbox, character_manager_.active_characters_.at(info_textbox.character_variable_)); //TODO : erreur si le personnage n'a pas déjà été show alors que cela devrait marcher dans ce cas là
		character_manager_.update_characters_dialogue(info_textbox);

		//textbox_manager_.dialogue_instruction_ = {TextboxManager::Where::none, false, false};
	}
	else if(std::holds_alternative<Script::InfoTransition>(current_script_information))
	{
		transition_manager_.update(std::get<Script::InfoTransition>(current_script_information));
	}
	else if(std::holds_alternative<Script::InfoChoiceMenu>(current_script_information))
	{
		choice_menu_manager_.update(std::get<Script::InfoChoiceMenu>(current_script_information));
		is_choice_menu_visible_ = true;
	}
}

void ScriptRunner::update()
{
	//std::cout << "CURRENT: " << current_script_index_ << std::endl;
	//std::cout << script_index_when_prev_.saved_script_index_ << std::endl;
	//std::cout << std::boolalpha << autofocus_manager_.autofocus_ << std::endl;

	if(textbox_manager_.dialogue_instruction_.where_ == TextboxManager::Where::PREV)
	{
		rebuild();
	}

	if(!is_choice_menu_visible_)
	{
		textbox_manager_.update_skip_auto_modes(); //car doit être exécuté avant move_dialogue()
	}

	if(textbox_manager_.skip_mode_) //don't play background transition in skip mode
	{
		transition_manager_.reset();
	}

	if(is_choice_menu_visible_ && choice_menu_manager_.choice_made_)
	{
		is_choice_menu_visible_ = false;
		is_dialogue_of_choice_menu_visible_ = false;
		textbox_manager_.dialogue_instruction_ = {TextboxManager::Where::NEXT, false, false};
	}

	if(!transition_manager_.transition_playing_)
	{
		move_dialogue(textbox_manager_.dialogue_instruction_.where_, textbox_manager_.dialogue_instruction_.is_from_mouse_wheel_);
	}
	else if(transition_manager_.transition_playing_ && transition_manager_.transition_->first_part_finished_ && !transition_manager_.new_background_displayed_)
	{
		move_dialogue(textbox_manager_.dialogue_instruction_.where_, textbox_manager_.dialogue_instruction_.is_from_mouse_wheel_);
		transition_manager_.new_background_displayed_ = true;
	}

	init_to_first_dialogue(); //doit être après move_dialogue()

	apply_line(current_script_index_); //TODO : aucun intérêt de passer un membre en paramètre...
	//move_dialogue(textbox_manager_.dialogue_instruction_.where_, textbox_manager_.dialogue_instruction_.is_from_mouse_wheel_);
	
	character_manager_.update_characters();
	transition_manager_.update_transition();
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
	transition_manager_.reset();
	choice_menu_manager_.reset();
	is_choice_menu_visible_ = false;
	is_dialogue_of_choice_menu_visible_ = false;


	//TODO : mettre dans une fonction
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
		else if(std::holds_alternative<Script::InfoChoiceMenu>(current_script_information))
		{
			choice_menu_manager_.update(std::get<Script::InfoChoiceMenu>(current_script_information));
		}
		//ne pas rejouer les transitions
	}
	save_current_script_index_when_scroll_back();
	play_all_sounds_before_previous_dialogue(target_script_index);
	handle_music_when_scroll_back(target_script_index);
	
	textbox_manager_.dialogue_instruction_ = {TextboxManager::Where::NONE, false, false};
	current_script_index_ = target_script_index;
}