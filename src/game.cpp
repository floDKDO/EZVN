#include "game.h"

#include <iostream>

Game::Game()
	: main_menu(nullptr), settings_menu(nullptr), load_menu(nullptr), save_menu(nullptr), 
	window("EZVN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE), 
	renderer(this->window.Get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), game_controller()
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

	this->textbox = new Textbox({255, 255, 255, 255}, this->renderer.Get());
	this->create_main_menu();
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

	SDL_StopTextInput();
	TTF_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::create_main_menu()
{
	Ui* button1 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 400, 100, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button2 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 620, 100, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button3 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 300, 320, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button4 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 520, 320, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button5 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 740, 320, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button6 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 520, 540, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button7 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 100, 540, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* slider1 = new Slider("img/slider_bar.png", "img/slider_handle.png", 0, 100, 800, 620, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* textbtn = new TextButton("tetetete", {255, 255, 255, 255}, {255, 255, 255, 255}, {255, 255, 255, 255}, 200, 200, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* inputfield = new Inputfield("img/inputfield.png", {0, 0, 0, 255}, 7, 100, 100, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));

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
	ui.push_back(textbtn);
	ui.push_back(inputfield);

	this->textbox->text.text = "Come on PLAYER! Maybe literature isn\'t that boring.";

	this->main_menu = new Menu(ui, button1);
}

void Game::create_settings_menu()
{
	Ui* button1 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 400, 100, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button2 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 620, 100, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button3 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 300, 320, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button4 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 520, 320, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button5 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 740, 320, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button6 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 520, 540, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));
	Ui* button7 = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 100, 540, renderer.Get(), std::bind(&Game::button_function, this, std::placeholders::_1));

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
	this->renderer.clear();
	this->get_state()->draw(renderer.Get());
	this->textbox->draw(renderer.Get());
	this->renderer.present();
}

void Game::update(Uint64& time_step)
{
	this->get_state()->update(time_step);
	this->textbox->update(time_step);
}

