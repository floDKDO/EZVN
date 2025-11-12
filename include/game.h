#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stack>

#include "gamestate.h"
#include "menu.h"
#include "textbutton.h"
#include "textbox.h"
#include "inputfield.h"
#include "SDL/window.h"
#include "SDL/renderer.h"
#include "SDL/gamecontroller.h"

class Game
{
	public:
		Game();
		~Game();
		Game(const Game& g) = delete;
		Game& operator=(const Game& g) = delete;

		void create_main_menu();
		void create_settings_menu();
		void button_function(Ui* ui);

		void push_state(GameState* state);
		void pop_state(); //TODO : doit retourner un GameState* ??
		GameState* get_state();

		void handle_events();
		void draw();
		void update(Uint64& time_step);

		Menu* main_menu;
		Menu* settings_menu;
		Menu* load_menu;
		Menu* save_menu;

		bool game_running;
		sdl::Window window;

		Textbox* textbox;

	protected:

	private:
		sdl::Renderer renderer;
		sdl::GameController game_controller;
		std::stack<GameState*> states;

		static const int WINDOW_WIDTH = 1280; 
		static const int WINDOW_HEIGHT = 720;
};

