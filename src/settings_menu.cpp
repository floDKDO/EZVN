#include "settings_menu.h"
#include "GUI/text_button.h"
#include "GUI/text_toggle.h"
#include "GUI/slider.h"
#include "game.h"

#include <iostream>
#include <sstream>
#include <set>

SettingsMenu::SettingsMenu(Game& game, std::string_view background_path, sdl::Renderer& renderer)
	: GameState(game), background_(background_path, 0, 0, renderer)
{

	//std::cout << SDL_GetNumVideoDisplays() << std::endl; //nombre d'écrans
	//std::cout << SDL_GetNumDisplayModes(0) << std::endl; //nombre de display modes

	scroll_ = std::make_unique<ScrollableArea>(600, 400, 300, 400, renderer);
	ui_group_ = std::make_unique<UiGroup>(600, 400);

	SDL_DisplayMode mode;
	SDL_GetWindowDisplayMode(game.window_.fetch(), &mode); //taille du contenu dans la fenêtre
	//std::cout << mode.h << ", " << mode.w << ", " << mode.refresh_rate << "FPS" << std::endl;

	SDL_GetCurrentDisplayMode(0, &mode); //résolution
	//std::cout << mode.h << ", " << mode.w << ", " << mode.refresh_rate << "FPS" << std::endl;

	std::cout << std::endl;
	int num = SDL_GetNumDisplayModes(0);
	std::set<std::pair<int, int>> resolutions;
	for(int i = 0; i < num; ++i)
	{
		SDL_GetDisplayMode(0, i, &mode);
		if(!resolutions.count({mode.h, mode.w}))
		{
			resolutions.insert({mode.h, mode.w});
			std::string resolution = std::to_string(mode.h) + 'x' + std::to_string(mode.w);
			bool checked = false;
			if(mode.h == constants::window_height_ && mode.w == constants::window_width_)
			{
				checked = true;
			}
			ui_group_->add_ui_element(std::make_unique<TextToggle>(resolution, 600, 400 + i * 10, checked, renderer, std::bind(&SettingsMenu::texttoggle_resolution_function, this, std::placeholders::_1)));
			std::cout << mode.h << ", " << mode.w << ", " << mode.refresh_rate << "FPS" << std::endl;
		}
	}
	scroll_->add_ui_element(std::move(ui_group_));
	std::cout << "Number: " << resolutions.size() << std::endl;
	std::cout << std::endl;

	build_ui_elements(renderer);
}

void SettingsMenu::build_ui_elements(sdl::Renderer& renderer)
{
	ui_manager_.add_element(std::make_unique<TextButton>("Return", 200, 500, renderer, std::bind(&SettingsMenu::previous_menu_function, this, std::placeholders::_1)));
	ui_manager_.add_element(std::make_unique<Slider>(0, 100, 100, 800, 200, "Sound effect", renderer, std::bind(&SettingsMenu::slider_sound_function, this, std::placeholders::_1)));
	ui_manager_.add_element(std::make_unique<Slider>(0, 100, 100, 450, 200, "Music effect", renderer, std::bind(&SettingsMenu::slider_music_function, this, std::placeholders::_1)));
	ui_manager_.add_element(std::make_unique<Slider>(30, 60, Text::global_text_divisor_, 625, 350, "Text speed", renderer, std::bind(&SettingsMenu::slider_text_function, this, std::placeholders::_1)));
	//ui_manager_.add_element(std::make_unique<TextToggleGroup<2>>("Display", std::vector<std::string>{"Windowed", "Fullscreen"}, 50, 100, true, renderer, std::vector<std::function<void(Ui* ui)>>{std::bind(&SettingsMenu::texttoggle_windowed_function, this, std::placeholders::_1), std::bind(&SettingsMenu::texttoggle_full_screen_function, this, std::placeholders::_1)}));
	//ui_manager_.add_element(std::make_unique<CheckboxGroup<2>>("Display", std::vector<std::string>{"Windowed", "Fullscreen"}, 50, 100, true, renderer, std::vector<std::function<void(Ui* ui)>>{std::bind(&Game::texttoggle_windowed_function, this, std::placeholders::_1), std::bind(&Game::texttoggle_full_screen_function, this, std::placeholders::_1)}));

	//TODO : essayer de trouver un meilleur moyen que d'utiliser "new"
	std::unique_ptr<UiGroup> ui_group = std::make_unique<UiGroup>("Display", 50, 100, renderer);
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
	SDL_GetWindowDisplayMode(game_.window_.fetch(), &display_mode);

	//TODO : std::istringstream ??
	std::stringstream stream(texttoggle_resolution->text_.text_);
	std::string h_res, w_res;

	//ne marche que si texttoggle_resolution->text_.text_ est de la forme h x w !
	std::getline(stream, h_res, 'x');
	std::getline(stream, w_res, 'x');
	std::cout << h_res << ", " << w_res << std::endl;

	display_mode.h = std::stoi(h_res);
	display_mode.w = std::stoi(w_res);
	std::cout << display_mode.h << ", " << display_mode.w << ", " << display_mode.refresh_rate << std::endl;

	//TODO : créer des méthodes is_fullscreen etc.
	if(SDL_GetWindowFlags(game_.window_.fetch()) & SDL_WINDOW_FULLSCREEN) //modif uniquement si la fenêtre est en plein écran
	{
		game_.window_.set_windowed();
		game_.window_.set_display_mode(&display_mode);
		game_.window_.set_full_screen();
	}
	else if(!(SDL_GetWindowFlags(game_.window_.fetch()) & SDL_WINDOW_FULLSCREEN)) //si pas plein écran => redimensionner la fenêtre
	{
		game_.window_.set_size(display_mode.w, display_mode.h);
	}
	
	std::cout << "Clicked on " << texttoggle_resolution->text_.text_ << std::endl;
}