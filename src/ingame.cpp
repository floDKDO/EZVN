#include "ingame.h"
#include "constants.h"
#include "game.h"

#include <iostream>

//TODO : le code des dialogues devra être modifié quand il y aura l'ajout de pauses, animations d'images, choice menus etc.

InGame::InGame(Game& game, sdl::Renderer& renderer)
	: GameState(game), script_runner_(game.script_, game_.audio_manager_, renderer), skip_mode_(false), auto_mode_(false), 
	autofocus_(constants::default_autofocus), /*textbox_(renderer),*/ hide_ui_textbox_(false), renderer_(renderer)
{
	build_ui_elements(renderer); 
}

void InGame::build_ui_elements(sdl::Renderer& renderer)
{
	std::unique_ptr<Ui> history_ui = std::make_unique<TextButton>("History", 0, 0, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX);
	history_button_ = dynamic_cast<TextButton*>(history_ui.get());
	ui_manager_.add_element(std::move(history_ui));

	std::unique_ptr<Ui> skip_ui = std::make_unique<TextToggle>("Skip", 0, 0, false, renderer, std::bind(&InGame::skip_function, this, std::placeholders::_1), TextToggleKind::ON_TEXTBOX);
	skip_toggle_ = dynamic_cast<TextToggle*>(skip_ui.get());
	ui_manager_.add_element(std::move(skip_ui));

	std::unique_ptr<Ui> auto_ui = std::make_unique<TextToggle>("Auto", 0, 0, false, renderer, std::bind(&InGame::auto_function, this, std::placeholders::_1), TextToggleKind::ON_TEXTBOX);
	auto_toggle_ = dynamic_cast<TextToggle*>(auto_ui.get());
	ui_manager_.add_element(std::move(auto_ui));

	std::unique_ptr<Ui> save_ui = std::make_unique<TextButton>("Save", 0, 0, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX);
	save_button_ = dynamic_cast<TextButton*>(save_ui.get());
	ui_manager_.add_element(std::move(save_ui));

	std::unique_ptr<Ui> load_ui = std::make_unique<TextButton>("Load", 0, 0, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX);
	load_button_ = dynamic_cast<TextButton*>(load_ui.get());
	ui_manager_.add_element(std::move(load_ui));

	std::unique_ptr<Ui> settings_ui = std::make_unique<TextButton>("Settings", 0, 0, renderer, std::bind(&InGame::settings_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX);
	settings_button_ = dynamic_cast<TextButton*>(settings_ui.get());
	ui_manager_.add_element(std::move(settings_ui));

	set_position_ui_textbox("bottom"); //TODO : hardcodé

	ui_manager_.set_elements();
}

void InGame::set_position_ui_textbox(std::string_view where)
{
	//textbox_.set_textbox_position(where);

	/*int x_textbutton = textbox_.textbox_.position_.x + constants::textbox_ui_elements_x_delta_;
	int y_textbutton = textbox_.textbox_.position_.y + textbox_.textbox_.position_.h + constants::textbox_ui_elements_y_delta_;

	history_button_->text_.position_.x = x_textbutton;
	history_button_->text_.position_.y = y_textbutton;
	x_textbutton += history_button_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	skip_toggle_->text_.position_.x = x_textbutton;
	skip_toggle_->text_.position_.y = y_textbutton;
	x_textbutton += skip_toggle_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	auto_toggle_->text_.position_.x = x_textbutton;
	auto_toggle_->text_.position_.y = y_textbutton;
	x_textbutton += auto_toggle_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	save_button_->text_.position_.x = x_textbutton;
	save_button_->text_.position_.y = y_textbutton;
	x_textbutton += save_button_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	load_button_->text_.position_.x = x_textbutton;
	load_button_->text_.position_.y = y_textbutton;
	x_textbutton += load_button_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	settings_button_->text_.position_.x = x_textbutton;
	settings_button_->text_.position_.y = y_textbutton;*/
}

//Fonctions de callback///////////////////////////////////////////
void InGame::auto_function([[maybe_unused]] Ui* ui)
{
	auto_mode_ = !auto_mode_;
}

void InGame::skip_function([[maybe_unused]] Ui* ui)
{
	skip_mode_ = !skip_mode_;
}

void InGame::settings_function([[maybe_unused]] Ui* ui)
{
	std::cout << "Pressed Settings!" << std::endl;
	game_.request_push_state(constants::settings_menu_unique_id_);
}

void InGame::temp_function([[maybe_unused]] Ui* ui)
{
	std::cout << "Pressed!\n";
}
//////////////////////////////////////////////////////////////////

void InGame::handle_events(const SDL_Event& e)
{
	if(!hide_ui_textbox_)
	{
		script_runner_.handle_events(e);

		ui_manager_.handle_events(e);
		if(ui_manager_.is_mouse_on_ui_)
		{
			return; //si collision avec un textbutton, ne pas gérer les événements "clic" et "espace" de la Textbox (= ne pas passer au prochain dialogue)
		}
	}

	if(e.type == SDL_MOUSEBUTTONDOWN && (e.button.button == SDL_BUTTON_RIGHT || e.button.button == SDL_BUTTON_MIDDLE))
	{
		hide_ui_textbox_ = !hide_ui_textbox_;
	}
}

void InGame::draw(sdl::Renderer& renderer)
{
	script_runner_.draw(renderer);

	if(!hide_ui_textbox_)
	{
		//textbox_.draw(renderer);
		ui_manager_.draw(renderer);
	}
}

//Textbox
void InGame::update_textbox(Script::InfoTextbox& info_textbox)
{
	set_position_ui_textbox(info_textbox);
}

void InGame::update_characters_dialogue(Script::InfoDialogue& info_dialogue)
{
	//if(which_dialogue_from_where_.which_dialogue_ == ScriptRunner::Where::none) //TODO : ajouter && !textbox_.is_first_dialogue_ ??
	//{
	//	return;
	//}

	//TODO : attention : conflit avec l'autofocus
	/*for(auto& [key_character_variable, value_character] : character_manager_.active_characters_)
	{
		value_character.properties_.is_speaking_ = false;
	}*/

	//std::cout << "PERSO: " << info_dialogue.character_variable_ << ", texte: " << info_dialogue.t_ << std::endl;

	//Character& character = character_manager_.active_characters_.at(info_dialogue.character_variable_);
	/*character.properties_.is_speaking_ = true;*/ //TODO : attention : conflit avec l'autofocus
}

void InGame::update()
{
	script_runner_.update();

	if(!hide_ui_textbox_)
	{
		ui_manager_.update();
		//textbox_.update();
	}
}