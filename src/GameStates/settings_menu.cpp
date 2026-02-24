#include "GameStates/settings_menu.h"
#include "GUI/text_button.h"
#include "GUI/text_toggle.h"
#include "GUI/slider.h"
#include "game.h"
#include "RAII_SDL2/display.h"

#include <iostream>
#include <sstream>
#include <set>

SettingsMenu::SettingsMenu(Game& game, sdl::Renderer& renderer)
	: GameState(game), background_(constants::default_menu_background_, 0, 0, renderer)
{
	create_resolutions_scroll_area(renderer);
	build_ui_elements(renderer);
}

void SettingsMenu::create_resolutions_scroll_area(sdl::Renderer& renderer)
{
	scroll_ = std::make_unique<ScrollableArea>("Resolutions", 950, 170, 200, 500, renderer);
	ui_group_ = std::make_unique<UiGroup>(950, 170);

	SDL_DisplayMode window_mode;
	game_.window_.get_display_mode(&window_mode);

	std::set<std::pair<int, int>> resolutions;
	int num = sdl::display::get_num_display_modes();
	for(int i = 0; i < num; ++i)
	{
		SDL_DisplayMode mode;
		sdl::display::get_display_mode(i, &mode);

		if(!resolutions.count({mode.h, mode.w}))
		{
			resolutions.insert({mode.h, mode.w});

			bool checked = false;

			if(mode.h == window_mode.h && mode.w == window_mode.w) 
			{
				checked = true;
			}

			std::string resolution = std::to_string(mode.w) + 'x' + std::to_string(mode.h);
			ui_group_->add_ui_element(std::make_unique<TextToggle>(resolution, 0, 0, checked, renderer, std::bind(&SettingsMenu::texttoggle_resolution_function, this, std::placeholders::_1)));
		}
	}
	scroll_->add_ui_element(std::move(ui_group_));
}

void SettingsMenu::build_ui_elements(sdl::Renderer& renderer)
{
	ui_manager_.add_element(std::make_unique<TextButton>("Return", 100, 550, renderer, std::bind(&SettingsMenu::previous_menu_function, this, std::placeholders::_1)));
	ui_manager_.add_element(std::make_unique<Slider>(0, 100, 100, 450, 170, "Music effect", renderer, std::bind(&SettingsMenu::slider_music_function, this, std::placeholders::_1)));
	ui_manager_.add_element(std::make_unique<Slider>(0, 100, 100, 450, 300, "Sound effect", renderer, std::bind(&SettingsMenu::slider_sound_function, this, std::placeholders::_1)));
	ui_manager_.add_element(std::make_unique<Slider>(30, 60, Text::global_text_divisor_, 450, 430, "Text speed", renderer, std::bind(&SettingsMenu::slider_text_function, this, std::placeholders::_1)));
	//ui_manager_.add_element(std::make_unique<TextToggleGroup<2>>("Display", std::vector<std::string>{"Windowed", "Fullscreen"}, 50, 100, true, renderer, std::vector<std::function<void(Ui* ui)>>{std::bind(&SettingsMenu::texttoggle_windowed_function, this, std::placeholders::_1), std::bind(&SettingsMenu::texttoggle_full_screen_function, this, std::placeholders::_1)}));
	//ui_manager_.add_element(std::make_unique<CheckboxGroup<2>>("Display", std::vector<std::string>{"Windowed", "Fullscreen"}, 50, 100, true, renderer, std::vector<std::function<void(Ui* ui)>>{std::bind(&Game::texttoggle_windowed_function, this, std::placeholders::_1), std::bind(&Game::texttoggle_full_screen_function, this, std::placeholders::_1)}));

	std::unique_ptr<UiGroup> ui_group = std::make_unique<UiGroup>("Display", 100, 100, renderer);
	ui_group->add_ui_element(std::make_unique<TextToggle>("Windowed", 0, 0, true, renderer, std::bind(&SettingsMenu::texttoggle_windowed_function, this, std::placeholders::_1)));
	ui_group->add_ui_element(std::make_unique<TextToggle>("Fullscreen", 0, 0, false, renderer, std::bind(&SettingsMenu::texttoggle_full_screen_function, this, std::placeholders::_1)));
	ui_manager_.add_element(std::move(ui_group));

	ui_manager_.add_element(std::move(scroll_));

	ui_manager_.set_elements();
}

void SettingsMenu::draw(sdl::Renderer& renderer)
{
	background_.draw(renderer);
	GameState::draw(renderer);
}

void SettingsMenu::update()
{
	background_.update();
	GameState::update();
}

void SettingsMenu::previous_menu_function([[maybe_unused]] Ui* ui)
{
	std::cout << "Pressed => go back to previous menu!" << std::endl;
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

void SettingsMenu::texttoggle_resolution_function(Ui* ui)
{
	std::cout << "CHANGE RESOLUTION\n";
	TextToggle* texttoggle_resolution = dynamic_cast<TextToggle*>(ui);

	SDL_DisplayMode display_mode;
	game_.window_.get_display_mode(&display_mode);

	std::istringstream stream(texttoggle_resolution->text_.text_);
	std::string h_res, w_res;

	//ne marche que si texttoggle_resolution->text_.text_ est de la forme w x h !
	std::getline(stream, w_res, 'x');
	std::getline(stream, h_res, 'x');
	//std::cout << h_res << ", " << w_res << std::endl;

	display_mode.h = std::stoi(h_res);
	display_mode.w = std::stoi(w_res);
	//std::cout << display_mode.h << ", " << display_mode.w << ", " << display_mode.refresh_rate << std::endl;

	if(game_.window_.is_full_screen()) //modif uniquement si la fenêtre est en plein écran
	{
		game_.window_.set_windowed();
		game_.window_.set_display_mode(&display_mode);
		game_.window_.set_full_screen();
	}
	else  //si pas plein écran => redimensionner la fenêtre
	{
		game_.window_.set_size(display_mode.w, display_mode.h);
		game_.window_.set_center(); //centrer la fenêtre après qu'elle ait été redimensionnée
	}
	
	std::cout << "Clicked on " << texttoggle_resolution->text_.text_ << std::endl;
}