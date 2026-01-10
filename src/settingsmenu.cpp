#include "settingsmenu.h"
#include "GUI/textbutton.h"
#include "GUI/slider.h"
#include "GUI/texttogglegroup.h"
#include "RAII_SDL2/music.h"
#include "game.h"

#include <iostream>

SettingsMenu::SettingsMenu(Game& game, const std::string_view background_path, sdl::Renderer& renderer)
	: GameState(game), background_(background_path, 0, 0, renderer)
{
	build_ui_elements(renderer);
}

void SettingsMenu::build_ui_elements(sdl::Renderer& renderer)
{
	ui_manager_.add_element(std::make_unique<TextButton>("Return", 200, 500, renderer, std::bind(&SettingsMenu::previous_menu_function, this, std::placeholders::_1)));
	ui_manager_.add_element(std::make_unique<Slider>(0, 100, 100, 800, 200, "Sound effect", renderer, std::bind(&SettingsMenu::slider_sound_function, this, std::placeholders::_1)));
	ui_manager_.add_element(std::make_unique<Slider>(0, 100, 100, 450, 200, "Music effect", renderer, std::bind(&SettingsMenu::slider_music_function, this, std::placeholders::_1)));
	ui_manager_.add_element(std::make_unique<Slider>(30, 60, Text::global_text_divisor_, 625, 350, "Text speed", renderer, std::bind(&SettingsMenu::slider_text_function, this, std::placeholders::_1)));
	ui_manager_.add_element(std::make_unique<TextToggleGroup<2>>("Display", std::vector<std::string>{"Windowed", "Fullscreen"}, 50, 100, true, renderer, std::vector<std::function<void(Ui* ui)>>{std::bind(&SettingsMenu::texttoggle_windowed_function, this, std::placeholders::_1), std::bind(&SettingsMenu::texttoggle_full_screen_function, this, std::placeholders::_1)}));
	//ui_manager_.add_element(std::make_unique<CheckboxGroup<2>>("Display", std::vector<std::string>{"Windowed", "Fullscreen"}, 50, 100, true, renderer, std::vector<std::function<void(Ui* ui)>>{std::bind(&Game::texttoggle_windowed_function, this, std::placeholders::_1), std::bind(&Game::texttoggle_full_screen_function, this, std::placeholders::_1)}));

	ui_manager_.set_elements();
}

void SettingsMenu::draw(sdl::Renderer& renderer)
{
	background_.draw(renderer);
	GameState::draw(renderer);
}

void SettingsMenu::previous_menu_function(Ui* ui)
{
	(void)ui;
	std::cout << "Clicked go back to previous menu!" << std::endl;
	game_.request_pop_state();
}

void SettingsMenu::slider_sound_function(Ui* ui)
{
	Slider* slider = dynamic_cast<Slider*>(ui);
	std::cout << "Changed value (" << slider->current_value_ << ") of slider!" << std::endl;
	game_.audio_manager_.set_volume_channel(-1, slider->current_value_);
}

void SettingsMenu::slider_music_function(Ui* ui)
{
	Slider* slider = dynamic_cast<Slider*>(ui);
	std::cout << "Changed value (" << slider->current_value_ << ") of slider!" << std::endl;
	game_.audio_manager_.set_volume_music(slider->current_value_);
}

void SettingsMenu::slider_text_function(Ui* ui)
{
	Slider* slider = dynamic_cast<Slider*>(ui);
	std::cout << "Changed value (" << slider->current_value_ << ") of slider!" << std::endl;
	Text::global_text_divisor_ = slider->current_value_;
}

void SettingsMenu::texttoggle_full_screen_function(Ui* ui)
{
	std::cout << "FULL SCREEN\n";
	TextToggle* texttoggle_full_screen = dynamic_cast<TextToggle*>(ui);
	if(!texttoggle_full_screen->is_checked_)
	{
		game_.window_.set_full_screen();
	}
}

void SettingsMenu::texttoggle_windowed_function(Ui* ui)
{
	std::cout << "WINDOWED SCREEN\n";
	TextToggle* texttoggle_windowed = dynamic_cast<TextToggle*>(ui);
	if(!texttoggle_windowed->is_checked_)
	{
		game_.window_.set_windowed();
	}
}