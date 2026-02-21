#include "Managers/textbox_manager.h"
#include "game.h"

#include <iostream>

TextboxManager::TextboxManager(sdl::Renderer& renderer, Game& game)
	: dialogue_instruction_({Where::NONE, false, false}), skip_mode_(false), last_time_(0), auto_mode_(false), hide_ui_textbox_(false), where_(constants::default_textbox_position_),
	textbox_(renderer), ui_manager_(game.audio_manager_), history_button_(nullptr), skip_toggle_(nullptr), auto_toggle_(nullptr), save_button_(nullptr), 
	load_button_(nullptr), settings_button_(nullptr), game_(game), renderer_(renderer)
{
	build_ui_elements(renderer);
}

void TextboxManager::build_ui_elements(sdl::Renderer& renderer)
{
	std::unique_ptr<TextButton> history_ui = std::make_unique<TextButton>("History", 0, 0, renderer, std::bind(&TextboxManager::temp_function, this, std::placeholders::_1), TextButton::Kind::ON_TEXTBOX);
	history_button_ = history_ui.get();
	ui_manager_.add_element(std::move(history_ui));

	std::unique_ptr<TextToggle> skip_ui = std::make_unique<TextToggle>("Skip", 0, 0, false, renderer, std::bind(&TextboxManager::skip_function, this, std::placeholders::_1), TextToggle::Kind::ON_TEXTBOX);
	skip_toggle_ = skip_ui.get();
	ui_manager_.add_element(std::move(skip_ui));

	std::unique_ptr<TextToggle> auto_ui = std::make_unique<TextToggle>("Auto", 0, 0, false, renderer, std::bind(&TextboxManager::auto_function, this, std::placeholders::_1), TextToggle::Kind::ON_TEXTBOX);
	auto_toggle_ = auto_ui.get();
	ui_manager_.add_element(std::move(auto_ui));

	std::unique_ptr<TextButton> save_ui = std::make_unique<TextButton>("Save", 0, 0, renderer, std::bind(&TextboxManager::temp_function, this, std::placeholders::_1), TextButton::Kind::ON_TEXTBOX);
	save_button_ = save_ui.get();
	ui_manager_.add_element(std::move(save_ui));

	std::unique_ptr<TextButton> load_ui = std::make_unique<TextButton>("Load", 0, 0, renderer, std::bind(&TextboxManager::temp_function, this, std::placeholders::_1), TextButton::Kind::ON_TEXTBOX);
	load_button_ = load_ui.get();
	ui_manager_.add_element(std::move(load_ui));

	std::unique_ptr<TextButton> settings_ui = std::make_unique<TextButton>("Settings", 0, 0, renderer, std::bind(&TextboxManager::settings_function, this, std::placeholders::_1), TextButton::Kind::ON_TEXTBOX);
	settings_button_ = settings_ui.get();
	ui_manager_.add_element(std::move(settings_ui));

	set_position_ui_textbox(constants::default_textbox_position_); 

	ui_manager_.set_elements();
}

void TextboxManager::set_position_ui_textbox(std::string_view where)
{
	where_ = where;

	textbox_.set_textbox_position(where);

	int x_textbutton = textbox_.textbox_.position_.x + constants::textbox_ui_elements_x_delta_;
	int y_textbutton = textbox_.textbox_.position_.y + textbox_.textbox_.position_.h + constants::textbox_ui_elements_y_delta_;

	history_button_->change_position(x_textbutton, y_textbutton);
	x_textbutton += history_button_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	skip_toggle_->change_position(x_textbutton, y_textbutton);
	x_textbutton += skip_toggle_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	auto_toggle_->change_position(x_textbutton, y_textbutton);
	x_textbutton += auto_toggle_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	save_button_->change_position(x_textbutton, y_textbutton);
	x_textbutton += save_button_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	load_button_->change_position(x_textbutton, y_textbutton);
	x_textbutton += load_button_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	settings_button_->change_position(x_textbutton, y_textbutton);
}

void TextboxManager::handle_events_mouse_wheel(const SDL_Event& e)
{
	if(e.type == SDL_MOUSEWHEEL) //condition placée en premier pour que le scroll de la mouse wheel sur un textbutton fonctionne
	{
		if(e.wheel.y > 0) //scroll vers l'avant => reculer d'un dialogue
		{
			//annuler le mode skip
			skip_toggle_->change_checked(false); 
			skip_mode_ = false;

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
		//annuler le mode auto
		auto_toggle_->change_checked(false); 
		auto_mode_ = false;

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
	if(!hide_ui_textbox_)
	{
		handle_events_mouse_wheel(e);
		ui_manager_.handle_events(e);
		if(ui_manager_.is_mouse_on_widget_)
		{
			return; //si collision avec un textbutton, ne pas gérer les événements "clic" et "espace" de la Textbox (= ne pas passer au prochain dialogue)
		}
		handle_events_keyboard_mouse(e);
	}

	if(e.type == SDL_MOUSEBUTTONDOWN && (e.button.button == SDL_BUTTON_RIGHT || e.button.button == SDL_BUTTON_MIDDLE))
	{
		hide_ui_textbox_ = !hide_ui_textbox_;
	}
}

void TextboxManager::draw(sdl::Renderer& renderer)
{
	if(!hide_ui_textbox_)
	{
		textbox_.draw(renderer);
		ui_manager_.draw(renderer);
	}
}

//Fonctions de callback///////////////////////////////////////////
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

void TextboxManager::update(const Script::InfoTextbox& info_textbox, const Character& character)
{
	//update_skip_auto_modes();

	if(!hide_ui_textbox_)
	{
		ui_manager_.update();
		textbox_.update();
	}

	if(info_textbox.t_.textbox_command_kind_ == Script::TextboxCommandKind::DIALOGUE)
	{
		if(dialogue_instruction_.where_ == Where::NONE && !textbox_.is_first_dialogue_)
		{
			return;
		}

		//std::cout << "*************************PERSO: " << info_textbox.character_variable_ << ", texte: " << info_textbox.t_.textbox_command_value_ << std::endl;
		textbox_.show_new_dialogue(info_textbox.t_.textbox_command_value_, character.properties_.name_, skip_mode_, dialogue_instruction_.wait_for_end_of_dialogue_);
		textbox_.change_textbox(character.properties_.textbox_path_, renderer_);
		textbox_.change_namebox(character.properties_.namebox_path_, renderer_);
		textbox_.change_namebox_text_color(character.properties_.namebox_text_color_); 
		//dialogue_instruction_ = {Where::none, false, false};

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
}