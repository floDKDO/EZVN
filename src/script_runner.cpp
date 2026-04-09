#include "script_runner.h"
#include "RAII_SDL2/pref_path.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;


ScriptRunner::ScriptRunner(TextboxManager::UiOnTextbox ui_on_textbox, UiManager& ui_manager, Game& game, sdl::Renderer& renderer)
	: current_script_index_(0), script_index_when_prev_({false, current_script_index_}), script_(game.script_), init_on_first_dialogue_(false), is_dialogue_of_choice_menu_visible_(false), are_ui_hidden_(false),
	ui_manager_(ui_manager), character_manager_(renderer, game.script_.character_definitions_), background_manager_(renderer), 
	music_manager_(game.audio_manager_), sound_manager_(game.audio_manager_), textbox_manager_(ui_manager, ui_on_textbox, renderer, game), choice_menu_manager_(ui_manager, renderer, game)
{
	create_save_file();
	init_dialogues_script_index();
}

void ScriptRunner::create_save_file()
{
	sdl::PrefPath pref_path;
	save_filename_ = pref_path.get_pref_path() + '\\' + std::string(constants::save_file_);
	std::ofstream output_file(save_filename_, std::ios_base::app);
	output_file.close();
}

void ScriptRunner::init_dialogues_script_index()
{
	for(size_t i = 0; i < script_.script_information_.size(); ++i)
	{
		if(std::holds_alternative<Script::InfoTextbox>(script_.script_information_[i]) 
		&& std::get<Script::InfoTextbox>(script_.script_information_[i]).t_.textbox_command_kind_ == Script::TextboxCommandKind::DIALOGUE)
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

void ScriptRunner::update_script_index()
{
	//std::cout << "AVANT: " << current_script_index_ << ", " << std::boolalpha << (where == TextboxManager::Instruction::NONE) << std::endl;
	if(textbox_manager_.is_dialogue_instruction_next() /*&& !is_dialogue_of_choice_menu_visible_*/)
	{
		if(textbox_manager_.is_input_source_mouse_wheel() && current_script_index_ < script_index_when_prev_.saved_script_index_)
		{
			increment_script_index();
		}
		else if(!textbox_manager_.is_input_source_mouse_wheel() && !is_dialogue_of_choice_menu_visible_)
		{
			script_index_when_prev_.is_saved_ = false; //when we pass a dialogue, reset the mouse wheel dialogues
			increment_script_index();
		}
	}
	else if(textbox_manager_.is_dialogue_instruction_prev())
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
	if(!init_on_first_dialogue_)
	{
		if(!is_current_script_index_a_dialogue())
		{
			textbox_manager_.go_to_next_dialogue();
		}
		else
		{
			//textbox_manager_.reset_dialogue_instruction(); //TODO : utile ??
			init_on_first_dialogue_ = true;
		}
	}
}

void ScriptRunner::handle_events(const SDL_Event& e)
{
	if(!are_ui_hidden_)
	{
		textbox_manager_.handle_events_mouse_wheel(e);
		ui_manager_.handle_events(e);
		if(!ui_manager_.is_mouse_on_widget_ && !ui_manager_.is_modal_ui_visible()) //uniquement s'il n'y a pas de collision avec un textbutton, gérer les événements "clic" et "espace" de la Textbox (= passer au prochain dialogue)
		{
			textbox_manager_.handle_events_keyboard_mouse(e);
		}
	}

	transition_manager_.handle_events(e);

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
	else if(e.type == SDL_MOUSEBUTTONDOWN && (e.button.button == SDL_BUTTON_RIGHT || e.button.button == SDL_BUTTON_MIDDLE))
	{
		if(!textbox_manager_.skip_mode_) //on ne peut pas cacher les UI quand on est en mode Skip
		{
			are_ui_hidden_ = !are_ui_hidden_;
		}
	}
}

void ScriptRunner::draw(sdl::Renderer& renderer)
{
	background_manager_.draw(renderer);
	character_manager_.draw(renderer);

	if(!are_ui_hidden_)
	{
		if(current_script_index_ != 0 && std::holds_alternative<Script::InfoChoiceMenu>(script_.script_information_[current_script_index_ - 1])) //TODO : utile ?
		{
			choice_menu_manager_.draw(renderer);
		}

		if(init_on_first_dialogue_ && !transition_manager_.transition_playing_)
		{
			textbox_manager_.draw(renderer);
			ui_manager_.draw(renderer);
		}
	}

	transition_manager_.draw(renderer);
}

//TODO : utiliser std::visit ??
void ScriptRunner::apply_script_line()
{
	Script::ScriptInformation& current_script_information = script_.script_information_[current_script_index_];
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
		if(choice_menu_manager_.is_visible_)
		{
			textbox_manager_.uncheck_auto_toggle();
			textbox_manager_.uncheck_skip_toggle();
		}

		Script::InfoTextbox& info_textbox = std::get<Script::InfoTextbox>(current_script_information);
		textbox_manager_.update(info_textbox, character_manager_.active_characters_.at(info_textbox.character_variable_)); //TODO : erreur si le personnage n'a pas déjà été show alors que cela devrait marcher dans ce cas là
		character_manager_.update_characters_dialogue(info_textbox);

		if(choice_menu_manager_.is_visible_ && textbox_manager_.is_dialogue_finished())
		{
			is_dialogue_of_choice_menu_visible_ = true; 
		}
	}
	else if(std::holds_alternative<Script::InfoTransition>(current_script_information))
	{
		transition_manager_.update(std::get<Script::InfoTransition>(current_script_information));
	}
	else if(std::holds_alternative<Script::InfoChoiceMenu>(current_script_information))
	{
		choice_menu_manager_.update(std::get<Script::InfoChoiceMenu>(current_script_information));
	}
}

void ScriptRunner::choice_made_from_choice_menu()
{
	if(is_dialogue_of_choice_menu_visible_)
	{
		if((!textbox_manager_.is_input_source_mouse_wheel() && choice_menu_manager_.choice_made_)
		|| (textbox_manager_.is_input_source_mouse_wheel() && textbox_manager_.is_dialogue_instruction_next() 
			&& script_index_when_prev_.saved_script_index_ > current_script_index_)) //on a scrollé pour passer le choice menu et on avait déjà fait un choix auparavant (<=> revenu avant le choice menu et scroll avant pour dépasser le choice menu)
		{
			choice_menu_manager_.hide();
			is_dialogue_of_choice_menu_visible_ = false;

			if(!textbox_manager_.is_input_source_mouse_wheel())
			{
				//reset du scroll de la souris après notre choix
				script_index_when_prev_.saved_script_index_ = current_script_index_;
				script_index_when_prev_.is_saved_ = true;
				textbox_manager_.go_to_next_dialogue();

				//dialogue qui suit le choix => on le modifie avec les bonnes valeurs selon le choix effectué
				Script::InfoTextbox& after_choice_dialogue = std::get<Script::InfoTextbox>(script_.script_information_[current_script_index_ + 1]);
				after_choice_dialogue.character_variable_ = choice_menu_manager_.after_choice_dialogue_chosen_.character_variable_;
				after_choice_dialogue.t_.textbox_command_value_ = choice_menu_manager_.after_choice_dialogue_chosen_.dialogue_;
			}
		}
	}
	else
	{
		textbox_manager_.update_textbox();
	}
}

void ScriptRunner::update_managers()
{
	if(textbox_manager_.skip_mode_ || textbox_manager_.is_input_source_mouse_wheel()) //don't play background transition in skip mode or when we scroll with the mouse wheel
	{
		transition_manager_.reset();
	}
	else
	{
		transition_manager_.update_transition(); //peut être placé avant ou après
	}

	if(!are_ui_hidden_)
	{
		choice_menu_manager_.update_buttons();
		ui_manager_.update();
		textbox_manager_.update_skip_auto_modes(); //peut être placé avant ou après
	}

	character_manager_.update_characters(); //doit être placé après apply_script_line()
}

void ScriptRunner::update()
{
	if(textbox_manager_.is_dialogue_instruction_prev())
	{
		rebuild();
	}

	init_to_first_dialogue(); 

	choice_made_from_choice_menu();

	if(!transition_manager_.transition_playing_ 
	|| (transition_manager_.transition_playing_ && transition_manager_.is_first_part_finished()))
	{
		update_script_index();
	}

	apply_script_line();

	update_managers();
}

void ScriptRunner::save()
{
	std::ifstream input_file(save_filename_);
	json data;

	if(input_file.is_open())
	{
		data = json::parse(input_file, nullptr, false);

		if(data.is_discarded())
		{
			data = json::object();
		}
	}
	input_file.close();

	std::ofstream output_file(save_filename_);
	data["save"] = current_script_index_;
	output_file << data.dump(4);
	output_file.close();
}

void ScriptRunner::load()
{
	std::ifstream input_file(save_filename_);
	json data;

	if(input_file.is_open())
	{
		data = json::parse(input_file, nullptr, false);

		if(data.is_discarded())
		{
			data = json::object();
		}
	}
	input_file.close();

	if(data.contains("save"))
	{
		size_t v = int(data["save"]);
		rebuild(v);
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

//méthode appelée dans deux cas : scroll sur un dialogue précédent ou load 
void ScriptRunner::rebuild(size_t target_script_index)
{
	bool is_load = !textbox_manager_.is_dialogue_instruction_prev();
	if(textbox_manager_.is_dialogue_instruction_prev() && !get_script_index_of_previous_dialogue().has_value()) //cas où on tente d'aller à un dialogue précédent alors qu'on est déjà sur le premier dialogue
	{
		return;
	}

	if(!is_load)
	{
		target_script_index = get_script_index_of_previous_dialogue().value();
	}

	character_manager_.reset();
	textbox_manager_.reset(is_load);
	sound_manager_.reset();
	background_manager_.reset();
	transition_manager_.reset();
	choice_menu_manager_.reset();

	is_dialogue_of_choice_menu_visible_ = false;

	//TODO : mettre dans une fonction
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

			if(is_load && i != target_script_index) // "i != target_script_index" pour ne pas ajouter le dernier dialogue deux fois dans l'historique
			{
				textbox_manager_.update_history(info_textbox, character_manager_.active_characters_.at(info_textbox.character_variable_));
			}

			character_manager_.update_characters_dialogue(info_textbox);
		}
		else if(std::holds_alternative<Script::InfoChoiceMenu>(current_script_information) && i == target_script_index - 1)
		{
			choice_menu_manager_.update(std::get<Script::InfoChoiceMenu>(current_script_information));
		}
		//ne pas rejouer les transitions
	}
	save_current_script_index_when_scroll_back();
	play_all_sounds_before_previous_dialogue(target_script_index);
	handle_music_when_scroll_back(target_script_index);
	
	textbox_manager_.reset_dialogue_instruction();
	current_script_index_ = target_script_index;
}