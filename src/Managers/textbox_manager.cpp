#include "Managers/textbox_manager.h"
#include "game.h"
#include "GameStates/history_menu.h"

#include <iostream>

TextboxManager::TextboxManager(UiOnTextbox ui_on_textbox, sdl::Renderer& renderer, Game& game)
	: dialogue_instruction_({Where::NONE, false, false}), skip_mode_(false), last_time_(0), auto_mode_(false), where_(constants::default_textbox_position_),
	textbox_(renderer), ui_on_textbox_(ui_on_textbox), game_(game), renderer_(renderer)
{
	ui_on_textbox_.history_button_->callback_function_ = std::bind(&TextboxManager::history_function, this, std::placeholders::_1);
	ui_on_textbox_.save_button_->callback_function_ = std::bind(&TextboxManager::temp_function, this, std::placeholders::_1);
	ui_on_textbox_.load_button_->callback_function_ = std::bind(&TextboxManager::temp_function, this, std::placeholders::_1);
	ui_on_textbox_.settings_button_->callback_function_ = std::bind(&TextboxManager::settings_function, this, std::placeholders::_1);
	ui_on_textbox_.skip_toggle_->callback_function_ = std::bind(&TextboxManager::skip_function, this, std::placeholders::_1);
	ui_on_textbox_.auto_toggle_->callback_function_ = std::bind(&TextboxManager::auto_function, this, std::placeholders::_1);
	set_position_ui_textbox(constants::default_textbox_position_);
}

void TextboxManager::set_position_ui_textbox(std::string_view where)
{
	where_ = where;

	textbox_.set_textbox_position(where);

	int x_textbutton = textbox_.textbox_.position_.x + constants::textbox_ui_elements_x_delta_;
	int y_textbutton = textbox_.textbox_.position_.y + textbox_.textbox_.position_.h + constants::textbox_ui_elements_y_delta_;

	ui_on_textbox_.history_button_->change_position(x_textbutton, y_textbutton);
	x_textbutton += ui_on_textbox_.history_button_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	ui_on_textbox_.skip_toggle_->change_position(x_textbutton, y_textbutton);
	x_textbutton += ui_on_textbox_.skip_toggle_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	ui_on_textbox_.auto_toggle_->change_position(x_textbutton, y_textbutton);
	x_textbutton += ui_on_textbox_.auto_toggle_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	ui_on_textbox_.save_button_->change_position(x_textbutton, y_textbutton);
	x_textbutton += ui_on_textbox_.save_button_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	ui_on_textbox_.load_button_->change_position(x_textbutton, y_textbutton);
	x_textbutton += ui_on_textbox_.load_button_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	ui_on_textbox_.settings_button_->change_position(x_textbutton, y_textbutton);
}

std::string TextboxManager::get_dialogue()
{
	return textbox_.get_dialogue();
}

std::string TextboxManager::get_speaker_name()
{
	return textbox_.get_speaker_name();
}

void TextboxManager::uncheck_skip_toggle()
{
	ui_on_textbox_.skip_toggle_->change_checked(false);
	skip_mode_ = false;
}

void TextboxManager::uncheck_auto_toggle()
{
	ui_on_textbox_.auto_toggle_->change_checked(false);
	auto_mode_ = false;
}

void TextboxManager::handle_events_ui_manager(const SDL_Event& e)
{
	//ui_manager_.handle_events(e);
}

void TextboxManager::handle_events_mouse_wheel(const SDL_Event& e)
{
	if(e.type == SDL_MOUSEWHEEL) //condition placée en premier pour que le scroll de la mouse wheel sur un textbutton fonctionne
	{
		if(e.wheel.y > 0) //scroll vers l'avant => reculer d'un dialogue
		{
			uncheck_skip_toggle();
			dialogue_instruction_ = {Where::PREV, true, false};
		}
		else //scroll vers l'arrière => avancer d'un dialogue
		{
			dialogue_instruction_ = {Where::NEXT, true, false};
		}
	}
}

void TextboxManager::handle_events_keyboard_mouse(const SDL_Event& e)
{
	if((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
	|| (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT))
	{
		uncheck_auto_toggle();

		if(textbox_.text_.is_finished_)
		{
			dialogue_instruction_ = {Where::NEXT, false, true};
		}
		else
		{
			textbox_.text_.is_animated_ = false; //afficher le dialogue en entier après un clic / touche espace sur un dialogue en train de s'afficher
		}
	}
}

void TextboxManager::handle_events(const SDL_Event& e)
{
	handle_events_mouse_wheel(e);
	//handle_events_ui_manager(e);
	//if(ui_manager_.is_mouse_on_widget_)
	//{
	//	return; //si collision avec un textbutton, ne pas gérer les événements "clic" et "espace" de la Textbox (= ne pas passer au prochain dialogue)
	//}
	handle_events_keyboard_mouse(e);
}

void TextboxManager::draw(sdl::Renderer& renderer)
{
	textbox_.draw(renderer);
}

//Fonctions de callback///////////////////////////////////////////
void TextboxManager::history_function([[maybe_unused]] Ui* ui)
{
	std::cout << "Pressed History!" << std::endl;
	game_.request_push_state(constants::history_menu_unique_id_);
}

void TextboxManager::auto_function([[maybe_unused]] Ui* ui)
{
	auto_mode_ = !auto_mode_;
}

void TextboxManager::skip_function([[maybe_unused]] Ui* ui)
{
	skip_mode_ = !skip_mode_;
}

void TextboxManager::settings_function([[maybe_unused]] Ui* ui)
{
	std::cout << "Pressed Settings!" << std::endl;
	game_.request_push_state(constants::settings_menu_unique_id_);
}

void TextboxManager::temp_function([[maybe_unused]] Ui* ui)
{
	std::cout << "Pressed!\n";
}
//////////////////////////////////////////////////////////////////

void TextboxManager::update_skip_auto_modes()
{
	if(skip_mode_)
	{
		dialogue_instruction_ = {Where::NEXT, false, false};
	}

	if(auto_mode_)
	{
		//dialogue_instruction_ = {Where::none, false, false};
		if(textbox_.text_.is_finished_)
		{
			Uint64 now = SDL_GetTicks64();
			if(last_time_ == 0)
			{
				last_time_ = now;
			}

			if(now > last_time_ + textbox_.get_text_delay())
			{
				dialogue_instruction_ = {Where::NEXT, false, true};
				last_time_ = SDL_GetTicks64();
			}
		}
	}
}

void TextboxManager::update_textbox()
{
	textbox_.update();
}

void TextboxManager::update(const Script::InfoTextbox& info_textbox, const Character& character)
{
	//update_skip_auto_modes();

	if(info_textbox.t_.textbox_command_kind_ == Script::TextboxCommandKind::DIALOGUE)
	{
		if(dialogue_instruction_.where_ == Where::NONE && !textbox_.is_first_dialogue_)
		{
			return;
		}

		//std::cout << "PERSO: " << textbox_.get_speaker_name() << ", texte: " << textbox_.get_dialogue() << std::endl;
		
		//std::cout << "PERSO: " << character.properties_.name_ << ", texte: " << info_textbox.t_.textbox_command_value_ << std::endl;
		textbox_.show_new_dialogue(info_textbox.t_.textbox_command_value_, character.properties_.name_, skip_mode_, dialogue_instruction_.wait_for_end_of_dialogue_);
		textbox_.change_textbox(character.properties_.textbox_path_, renderer_);
		textbox_.change_namebox(character.properties_.namebox_path_, renderer_);
		textbox_.change_namebox_text_color(character.properties_.namebox_text_color_); 
		//dialogue_instruction_ = {Where::none, false, false};

		//if(dialogue_instruction_.is_from_mouse_wheel_ && dialogue_instruction_.where_ == Where::PREV || !dialogue_instruction_.is_from_mouse_wheel_)
		{
			HistoryMenu* history_menu = dynamic_cast<HistoryMenu*>(game_.get_state(constants::history_menu_unique_id_));
			history_menu->add_dialogue_to_history(get_speaker_name(), get_dialogue(), character.properties_.namebox_text_color_, textbox_.textbox_.position_.w, renderer_);
		}

		if(!skip_mode_)
		{
			dialogue_instruction_ = {Where::NONE, false, false};
		}
	}
	else if(info_textbox.t_.textbox_command_kind_ == Script::TextboxCommandKind::MOVE_TEXTBOX)
	{
		set_position_ui_textbox(info_textbox.t_.textbox_command_value_);
	}

	if(where_.empty())
	{
		set_position_ui_textbox(constants::default_textbox_position_);
	}
}

void TextboxManager::reset()
{
	textbox_.text_.text_.clear();
	textbox_.is_first_dialogue_ = true;
	where_.clear();

	HistoryMenu* history_menu = dynamic_cast<HistoryMenu*>(game_.get_state(constants::history_menu_unique_id_));
	history_menu->scroll_ptr_->remove_last_text();
}