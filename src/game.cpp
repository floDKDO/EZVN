#include "game.h"

#include <iostream>

//TODO : ne pas utiliser new mais des smart pointers

Game::Game()
	: sdl(SDL_INIT_EVERYTHING), sdl_img(IMG_INIT_PNG | IMG_INIT_JPG), sdl_mixer(MIX_INIT_OGG | MIX_INIT_MP3),
	main_menu(nullptr), settings_menu(nullptr), load_menu(nullptr), save_menu(nullptr), 
	window("EZVN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI),
	renderer(this->window.Get(), -1, SDL_RENDERER_PRESENTVSYNC), game_controller()
{
	this->renderer.set_logical_size(WINDOW_WIDTH, WINDOW_HEIGHT);
	this->renderer.set_draw_blend_mode(SDL_BLENDMODE_BLEND);

	this->textbox = new Textbox({255, 255, 255, 255}, this->renderer.Get());
	this->textbox->text.text = "Come on PLAYER! Maybe literature isn\'t that boring.";

	this->create_main_menu();
	this->create_settings_menu();
	this->push_state(this->main_menu);
}

Game::~Game()
{
	if(this->main_menu)
	{
		delete this->main_menu;
	}

	if(this->settings_menu)
	{
		delete this->settings_menu;
	}

	if(this->load_menu)
	{
		delete this->load_menu;
	}
	
	if(this->save_menu)
	{
		delete this->save_menu;
	}

	delete this->textbox;
}

void Game::create_main_menu()
{
	Ui* textbutton_play = new TextButton("Play", {255, 255, 255, 255}, {255, 0, 0, 255}, {255, 0, 0, 255}, 600, 200, renderer.Get(), std::bind(&Game::play_function, this, std::placeholders::_1));
	Ui* textbutton_settings = new TextButton("Settings", {255, 255, 255, 255}, {255, 0, 0, 255}, {255, 0, 0, 255}, 600, 350, renderer.Get(), std::bind(&Game::settings_function, this, std::placeholders::_1));
	Ui* textbutton_quit = new TextButton("Quit", {255, 255, 255, 255}, {255, 0, 0, 255}, {255, 0, 0, 255}, 600, 500, renderer.Get(), std::bind(&Game::quit_function, this, std::placeholders::_1));

	std::vector<Ui*> ui;
	ui.reserve(10);

	ui.push_back(textbutton_play);
	ui.push_back(textbutton_settings);
	ui.push_back(textbutton_quit);

	this->main_menu = new Menu(ui, textbutton_play);
}

void Game::create_settings_menu()
{
	Ui* textbutton_return = new TextButton("Return", {255, 255, 255, 255}, {255, 0, 0, 255}, {255, 0, 0, 255}, 200, 500, renderer.Get(), std::bind(&Game::previous_menu_function, this, std::placeholders::_1));
	Ui* slider_sound = new Slider("img/gui/slider_bar.png", "img/gui/slider_handle.png", 0, 100, 800, 200, "Sound effect", renderer.Get(), std::bind(&Game::slider_sound_function, this, std::placeholders::_1));
	Ui* slider_music = new Slider("img/gui/slider_bar.png", "img/gui/slider_handle.png", 0, 100, 450, 200, "Music effect", renderer.Get(), std::bind(&Game::slider_music_function, this, std::placeholders::_1));
	Ui* togglegroup = new TextToggleGroup(2, "Display", {"Windowed", "Fullscreen"}, {200, 200, 200, 255}, {255, 255, 255, 255}, {255, 0, 0, 255}, 50, 100, true, renderer.Get(), {std::bind(&Game::texttoggle_windowed_function, this, std::placeholders::_1), std::bind(&Game::texttoggle_full_screen_function, this, std::placeholders::_1)});

	std::vector<Ui*> ui;
	ui.reserve(10);

	ui.push_back(textbutton_return);
	ui.push_back(slider_sound);
	ui.push_back(slider_music);
	ui.push_back(togglegroup);

	this->settings_menu = new Menu(ui, textbutton_return);
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
	this->push_state(this->settings_menu);
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
	TextToggle* texttoggle_full_screen = dynamic_cast<TextToggle*>(ui);
	if(!texttoggle_full_screen->is_checked)
	{
		this->window.set_full_screen();
	}

}

void Game::texttoggle_windowed_function(Ui* ui)
{
	//TODO
	std::cout << "WINDOWED SCREEN\n";
	TextToggle* texttoggle_windowed = dynamic_cast<TextToggle*>(ui);
	if(!texttoggle_windowed->is_checked)
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

	int w, h; //TODO : temp

	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_WINDOWEVENT:
				if(e.window.event == SDL_WINDOWEVENT_CLOSE)
				{
					game_running = false;
				}
				else if(e.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					//TODO
					/*std::cout << "RESIZED! " << e.window.data1 << " et " << e.window.data2 << std::endl;
					SDL_GetWindowSize(this->window.Get(), &w, &h);
					std::cout << "W : " << w << " et H : " << h << std::endl;
					SDL_RenderSetLogicalSize(this->renderer.Get(), e.window.data1, e.window.data2);
					SDL_RenderGetLogicalSize(this->renderer.Get(), &w, &h);
					std::cout << "W : " << w << " et H : " << h << std::endl;*/
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
	this->get_state()->draw(renderer.Get());
	this->textbox->draw(renderer.Get());
	this->renderer.present();
}

void Game::update(Uint64 time_step)
{
	this->get_state()->update(time_step);
	this->textbox->update(time_step);
}

