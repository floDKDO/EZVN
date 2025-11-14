#include "game.h"

#include <iostream>

Game::Game()
	: main_menu(nullptr), settings_menu(nullptr), load_menu(nullptr), save_menu(nullptr), 
	window("EZVN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE), 
	renderer(this->window.Get(), -1, SDL_RENDERER_PRESENTVSYNC), game_controller()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_CHANNELS, 2048);
	TTF_Init();

	SDL_StartTextInput();

	SDL_GameControllerEventState(SDL_ENABLE);

	this->renderer.set_logical_size(WINDOW_WIDTH, WINDOW_HEIGHT);
	this->renderer.set_draw_blend_mode(SDL_BLENDMODE_BLEND);

	//this->textbox = new Textbox({255, 255, 255, 255}, this->renderer.Get());
	//this->textbox->text.text = "Come on PLAYER! Maybe literature isn\'t that boring.";

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

	//delete this->textbox;

	SDL_StopTextInput();
	TTF_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
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
	Ui* slider = new Slider("img/gui/slider_bar.png", "img/gui/slider_handle.png", 0, 100, 600, 200, renderer.Get(), std::bind(&Game::slider_function, this, std::placeholders::_1));

	std::vector<Ui*> ui;
	ui.reserve(10);

	ui.push_back(textbutton_return);
	ui.push_back(slider);

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

void Game::slider_function(Ui* ui)
{
	Slider* slider = dynamic_cast<Slider*>(ui);
	std::cout << "Changed value (" << slider->current_value << ") of slider!" << std::endl;
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
						//this->textbox->show_new_dialogue("And then, I would be I good guy because they are a lot of people that like somebody that used to be.", "Sayori");
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
	//this->textbox->draw(renderer.Get());
	this->renderer.present();
}

void Game::update(Uint64 time_step)
{
	this->get_state()->update(time_step);
	//this->textbox->update(time_step);
}

