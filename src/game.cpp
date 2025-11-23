#include "game.h"
#include "music.h"
#include "GUI/slider.h"
#include "GUI/textbutton.h"
#include "GUI/texttoggle.h"
#include "GUI/texttogglegroup.h"

#include <iostream>

//TODO : garder les vectors de C-pointeurs ??

Game::Game()
	: sdl_(SDL_INIT_EVERYTHING), sdl_img_(IMG_INIT_PNG | IMG_INIT_JPG), sdl_mixer_(MIX_INIT_OGG | MIX_INIT_MP3), sdl_ttf_(),
	window_("EZVN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH_, WINDOW_HEIGHT_, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI),
	renderer_(window_, -1, SDL_RENDERER_PRESENTVSYNC),
	game_controller_(),
	main_menu_(nullptr), settings_menu_(nullptr), load_menu_(nullptr), save_menu_(nullptr)
{
	renderer_.set_logical_size(WINDOW_WIDTH_, WINDOW_HEIGHT_);
	renderer_.set_draw_blend_mode(SDL_BLENDMODE_BLEND);

	in_game_ = std::make_unique<InGame>(renderer_);
	create_main_menu();
	create_settings_menu();
	push_state(main_menu_.get());
}

void Game::create_main_menu()
{
	std::vector<std::unique_ptr<Ui>> ui_elements;
	ui_elements.reserve(10);

	ui_elements.push_back(std::make_unique<TextButton>("Play", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, 600, 200, renderer_, std::bind(&Game::play_function, this, std::placeholders::_1)));
	ui_elements.push_back(std::make_unique<TextButton>("Settings", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, 600, 350, renderer_, std::bind(&Game::settings_function, this, std::placeholders::_1)));
	ui_elements.push_back(std::make_unique<TextButton>("Quit", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, 600, 500, renderer_, std::bind(&Game::quit_function, this, std::placeholders::_1)));

	main_menu_ = std::make_unique<Menu>(std::move(ui_elements), ui_elements[0].get());
}

void Game::create_settings_menu()
{
	//std::unique_ptr<Ui> togglegroup = std::make_unique<TextToggleGroup>(2, "Display", std::vector<std::string>{"Windowed", "Fullscreen"}, SDL_Color{200, 200, 200, 255}, SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, 50, 100, true, renderer.Get(), std::vector<std::function<void(Ui* ui)>>{std::bind(&Game::texttoggle_windowed_function, this, std::placeholders::_1), std::bind(&Game::texttoggle_full_screen_function, this, std::placeholders::_1)});

	std::vector<std::unique_ptr<Ui>> ui_elements;
	ui_elements.reserve(10);
	 
	ui_elements.push_back(std::make_unique<TextButton>("Return", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, 200, 500, renderer_, std::bind(&Game::previous_menu_function, this, std::placeholders::_1)));
	ui_elements.push_back(std::make_unique<Slider>("img/gui/slider_bar.png", "img/gui/slider_handle.png", 0, 100, 800, 200, "Sound effect", renderer_, std::bind(&Game::slider_sound_function, this, std::placeholders::_1)));
	ui_elements.push_back(std::make_unique<Slider>("img/gui/slider_bar.png", "img/gui/slider_handle.png", 0, 100, 450, 200, "Music effect", renderer_, std::bind(&Game::slider_music_function, this, std::placeholders::_1)));
	ui_elements.push_back(std::make_unique<Slider>("img/gui/slider_bar.png", "img/gui/slider_handle.png", 30, 0, 625, 350, "Text speed", renderer_, std::bind(&Game::slider_text_function, this, std::placeholders::_1)));
	ui_elements.push_back(std::make_unique<TextToggleGroup<2>>("Display", std::vector<std::string>{"Windowed", "Fullscreen"}, SDL_Color{200, 200, 200, 255}, SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, 50, 100, true, renderer_, std::vector<std::function<void(Ui* ui)>>{std::bind(&Game::texttoggle_windowed_function, this, std::placeholders::_1), std::bind(&Game::texttoggle_full_screen_function, this, std::placeholders::_1)}));

	settings_menu_ = std::make_unique<Menu>(std::move(ui_elements), ui_elements[0].get());
}

void Game::play_function(Ui* ui)
{
	(void)ui;
	std::cout << "Clicked Play!" << std::endl;
	push_state(in_game_.get());
}

void Game::settings_function(Ui* ui)
{
	(void)ui;
	std::cout << "Clicked Settings!" << std::endl;
	push_state(settings_menu_.get());
}

void Game::quit_function(Ui* ui)
{
	(void)ui;
	std::cout << "Clicked Quit!" << std::endl;
	game_running_ = false;
}

void Game::previous_menu_function(Ui* ui)
{
	(void)ui;
	std::cout << "Clicked go back to previous menu!" << std::endl;
	pop_state();
}

void Game::slider_sound_function(Ui* ui)
{
	Slider* slider = dynamic_cast<Slider*>(ui);
	std::cout << "Changed value (" << slider->current_value_ << ") of slider!" << std::endl;
	Sound::global_sound_volume_ = slider->current_value_;
}

void Game::slider_music_function(Ui* ui)
{
	Slider* slider = dynamic_cast<Slider*>(ui);
	std::cout << "Changed value (" << slider->current_value_ << ") of slider!" << std::endl;
	Music::global_music_volume_ = slider->current_value_;
}

void Game::slider_text_function(Ui* ui)
{
	Slider* slider = dynamic_cast<Slider*>(ui);
	std::cout << "Changed value (" << slider->current_value_ << ") of slider!" << std::endl;
	Text::global_text_speed_ = slider->current_value_;
}

void Game::texttoggle_full_screen_function(Ui* ui)
{
	std::cout << "FULL SCREEN\n";
	TextToggle* texttoggle_full_screen = dynamic_cast<TextToggle*>(ui);
	if(!texttoggle_full_screen->is_checked_)
	{
		window_.set_full_screen();
	}
}

void Game::texttoggle_windowed_function(Ui* ui)
{
	std::cout << "WINDOWED SCREEN\n";
	TextToggle* texttoggle_windowed = dynamic_cast<TextToggle*>(ui);
	if(!texttoggle_windowed->is_checked_)
	{
		window_.set_windowed();
	}
}

void Game::push_state(GameState* state)
{
	states_.push(state);
}

void Game::pop_state()
{
	states_.pop();
}

GameState* Game::get_state()
{
	if(states_.empty())
		std::cerr << "VIDE!\n";
	return states_.top();
}

void Game::handle_events()
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_WINDOWEVENT:
				if(e.window.event == SDL_WINDOWEVENT_CLOSE)
				{
					game_running_ = false;
				}
				break;

			case SDL_KEYDOWN:
				switch(e.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						game_running_ = false;
						break;

					default:
						break;
				}
				break;

			case SDL_KEYUP:
				break;

			default:
				break;
		}
		get_state()->handle_events(e);
	}
}

void Game::draw()
{
	renderer_.clear();
	get_state()->draw(renderer_);
	renderer_.present();
}

void Game::update(Uint64 time_step)
{
	get_state()->update(time_step);
}

