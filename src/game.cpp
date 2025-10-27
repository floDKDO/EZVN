#include "game.h"

#include <iostream>
#include <chrono>
#include <thread>

Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_CHANNELS, 2048);
	TTF_Init();

	SDL_StartTextInput();

	this->controller = SDL_GameControllerOpen(0);
	SDL_GameControllerEventState(SDL_ENABLE);

	static const int WINDOW_WIDTH = 1280; //TODO : attributs ??
	static const int WINDOW_HEIGHT = 720;

	this->window = SDL_CreateWindow("EZVN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetLogicalSize(this->renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);

	this->create_main_menu();
	this->push_state(this->main_menu);
}

Game::~Game()
{
	if(this->controller != nullptr)
		SDL_GameControllerClose(this->controller);

	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
	SDL_StopTextInput();
	TTF_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::create_main_menu()
{
	Ui* button1 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 400, 100, renderer, std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button2 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 620, 100, renderer, std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button3 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 300, 320, renderer, std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button4 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 520, 320, renderer, std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button5 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 740, 320, renderer, std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button6 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 520, 540, renderer, std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button7 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 100, 540, renderer, std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* slider1 = new Slider("img/slider_bar.png", "img/slider_handle.png", 0, 100, 800, 620, renderer, std::bind(&Game::button_function, this, std::placeholders::_1));

	std::vector<Ui*> ui;
	ui.reserve(10);

	ui.push_back(button1);
	ui.push_back(button2);
	ui.push_back(button3);
	ui.push_back(button4);
	ui.push_back(button5);
	ui.push_back(button6);
	ui.push_back(button7);
	ui.push_back(slider1);

	this->main_menu = new Menu(ui, button1);
}

void Game::create_settings_menu()
{
	Ui* button1 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 400, 100, renderer, std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button2 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 620, 100, renderer, std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button3 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 300, 320, renderer, std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button4 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 520, 320, renderer, std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button5 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 740, 320, renderer, std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button6 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 520, 540, renderer, std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button7 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 100, 540, renderer, std::bind(&Game::button_function, this, std::placeholders::_1));

	std::vector<Ui*> ui;
	ui.reserve(10);

	ui.push_back(button1);
	ui.push_back(button2);
	ui.push_back(button3);
	ui.push_back(button4);
	ui.push_back(button5);
	ui.push_back(button6);
	ui.push_back(button7);

	this->settings_menu = new Menu(ui, button1);
}

void Game::button_function(Ui* ui)
{
	(void)ui;
	std::cout << "Clicked !" << std::endl;
	//this->pop_state(); //TODO : je pop mais la stack devient vide => pose problème dans les handle_events, draw et update
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

					default:
						break;
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
	SDL_RenderClear(renderer);
	this->get_state()->draw(renderer);
	SDL_RenderPresent(renderer);
}

void Game::update(Uint64& time_step)
{
	this->get_state()->update(time_step);
}

