#include "game.h"

#include <iostream>

//TODO : regarder les .get()
//TODO : garder les vectors de C-pointeurs ??

Game::Game()
	: sdl(SDL_INIT_EVERYTHING), sdl_img(IMG_INIT_PNG | IMG_INIT_JPG), sdl_mixer(MIX_INIT_OGG | MIX_INIT_MP3), sdl_ttf(),
	window("EZVN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI),
	renderer(this->window, -1, SDL_RENDERER_PRESENTVSYNC),
	game_controller(),
	main_menu(nullptr), settings_menu(nullptr), load_menu(nullptr), save_menu(nullptr)
{
	this->renderer.set_logical_size(WINDOW_WIDTH, WINDOW_HEIGHT);
	this->renderer.set_draw_blend_mode(SDL_BLENDMODE_BLEND);

	this->textbox = std::make_unique<Textbox>(SDL_Color{255, 255, 255, 255}, this->renderer);
	this->textbox->text.text = "Come on PLAYER! Maybe literature isn\'t that boring.";

	this->create_main_menu();
	this->create_settings_menu();
	this->push_state(this->main_menu.get());
}

void Game::create_main_menu()
{
	std::vector<std::unique_ptr<Ui>> ui;
	ui.reserve(10);

	ui.push_back(std::make_unique<TextButton>(std::string("Play"), SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, 600, 200, renderer, std::bind(&Game::play_function, this, std::placeholders::_1)));
	ui.push_back(std::make_unique<TextButton>("Settings", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, 600, 350, renderer, std::bind(&Game::settings_function, this, std::placeholders::_1)));
	ui.push_back(std::make_unique<TextButton>("Quit", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, 600, 500, renderer, std::bind(&Game::quit_function, this, std::placeholders::_1)));

	this->main_menu = std::make_unique<Menu>(std::move(ui), ui[0].get());
}

void Game::create_settings_menu()
{
	//std::unique_ptr<Ui> togglegroup = std::make_unique<TextToggleGroup>(2, "Display", std::vector<std::string>{"Windowed", "Fullscreen"}, SDL_Color{200, 200, 200, 255}, SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, 50, 100, true, renderer.Get(), std::vector<std::function<void(Ui* ui)>>{std::bind(&Game::texttoggle_windowed_function, this, std::placeholders::_1), std::bind(&Game::texttoggle_full_screen_function, this, std::placeholders::_1)});

	std::vector<std::unique_ptr<Ui>> ui;
	ui.reserve(10);

	ui.push_back(std::make_unique<TextButton>("Return", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, 200, 500, renderer, std::bind(&Game::previous_menu_function, this, std::placeholders::_1)));
	ui.push_back(std::make_unique<Slider>("img/gui/slider_bar.png", "img/gui/slider_handle.png", 0, 100, 800, 200, "Sound effect", renderer, std::bind(&Game::slider_sound_function, this, std::placeholders::_1)));
	ui.push_back(std::make_unique<Slider>("img/gui/slider_bar.png", "img/gui/slider_handle.png", 0, 100, 450, 200, "Music effect", renderer, std::bind(&Game::slider_music_function, this, std::placeholders::_1)));
	ui.push_back(std::make_unique<CheckboxGroup>(2, "Display", "img/gui/button_normal.png", "img/gui/button_selected.png", "img/gui/button_clicked.png", "img/gui/checked.png", 50, 100, true, renderer, std::vector<std::function<void(Ui * ui)>>{std::bind(&Game::texttoggle_windowed_function, this, std::placeholders::_1), std::bind(&Game::texttoggle_full_screen_function, this, std::placeholders::_1)}));

	this->settings_menu = std::make_unique<Menu>(std::move(ui), ui[0].get());
}

void Game::play_function(Ui* ui)
{
	(void)ui;
	std::cout << "Clicked Play!" << std::endl;
	//this->push_state(); //TODO : créer un état IN_GAME
}

void Game::settings_function(Ui* ui)
{
	(void)ui;
	std::cout << "Clicked Settings!" << std::endl;
	this->push_state(this->settings_menu.get());
}

void Game::quit_function(Ui* ui)
{
	(void)ui;
	std::cout << "Clicked Quit!" << std::endl;
	this->game_running = false;
}

void Game::previous_menu_function(Ui* ui)
{
	(void)ui;
	std::cout << "Clicked go back to previous menu!" << std::endl;
	this->pop_state();
	std::cout << "[0] -> is_checked: " << std::boolalpha << dynamic_cast<CheckboxGroup*>(this->settings_menu->ui[3].get())->checkables[0]->is_checked << ", previous_checked: " << dynamic_cast<CheckboxGroup*>(this->settings_menu->ui[3].get())->checkables[0]->previous_checked << "[1] -> is_checked: " << dynamic_cast<CheckboxGroup*>(this->settings_menu->ui[3].get())->checkables[1]->is_checked << ", previous_checked: " << dynamic_cast<CheckboxGroup*>(this->settings_menu->ui[3].get())->checkables[1]->previous_checked << std::endl;
}

void Game::slider_sound_function(Ui* ui)
{
	Slider* slider = dynamic_cast<Slider*>(ui);
	std::cout << "Changed value (" << slider->current_value << ") of slider!" << std::endl;
	Sound::global_sound_volume = slider->current_value;
}

void Game::slider_music_function(Ui* ui)
{
	Slider* slider = dynamic_cast<Slider*>(ui);
	std::cout << "Changed value (" << slider->current_value << ") of slider!" << std::endl;
	Music::global_music_volume = slider->current_value;
}

void Game::texttoggle_full_screen_function(Ui* ui)
{
	//TODO
	std::cout << "FULL SCREEN\n";
	/*TextToggle* texttoggle_full_screen = dynamic_cast<TextToggle*>(ui);
	if(!texttoggle_full_screen->is_checked)
	{
		this->window.set_full_screen();
	}*/

	Checkbox* checkbox_full_screen = dynamic_cast<Checkbox*>(ui);
	if(!checkbox_full_screen->is_checked)
	{
		this->window.set_full_screen();
	}

}

void Game::texttoggle_windowed_function(Ui* ui)
{
	//TODO
	std::cout << "WINDOWED SCREEN\n";
	/*TextToggle* texttoggle_windowed = dynamic_cast<TextToggle*>(ui);
	if(!texttoggle_windowed->is_checked)
	{
		this->window.set_windowed();
	}*/

	Checkbox* checkbox_windowed = dynamic_cast<Checkbox*>(ui);
	if(!checkbox_windowed->is_checked)
	{
		this->window.set_windowed();
	}
}

void Game::push_state(GameState* state)
{
	this->states.push(state);
}

void Game::pop_state()
{
	this->states.pop();
}

GameState* Game::get_state()
{
	if(this->states.empty())
		std::cerr << "VIDE!\n";
	return this->states.top();
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
					game_running = false;
				}
				break;

			case SDL_KEYDOWN:
				switch(e.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						game_running = false;
						break;

					case SDLK_SPACE:
						//Prochain dialogue
						this->textbox->show_new_dialogue("And then, I would be I good guy because they are a lot of people that like somebody that used to be.", "Sayori");
						break;

					default:
						break;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if(e.button.button == SDL_BUTTON_LEFT)
				{
					//TODO : si sur la textbox OU sur rien du tout, alors passer au dialogue suivant
				}
				break;

			case SDL_KEYUP:
				break;

			default:
				break;
		}
		this->get_state()->handle_events(e);
	}
}

void Game::draw()
{
	this->renderer.clear();
	this->get_state()->draw(renderer);
	this->textbox->draw(renderer);
	this->renderer.present();
}

void Game::update(Uint64 time_step)
{
	this->get_state()->update(time_step);
	this->textbox->update(time_step);
}

