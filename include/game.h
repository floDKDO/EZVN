#pragma once

#include "gamestate.h"
#include "menu.h"
#include "textbutton.h"
#include "textbox.h"
#include "inputfield.h"
#include "texttogglegroup.h"
#include "music.h"
#include "RAII_SDL2/window.h"
#include "RAII_SDL2/renderer.h"
#include "RAII_SDL2/gamecontroller.h"
#include "RAII_SDL2/sdl.h"
#include "RAII_SDL2/sdlmixer.h"
#include "RAII_SDL2/sdlimg.h"
#include "RAII_SDL2/sdlttf.h"

#include <memory>
#include <stack>

class Game
{
	public:
		Game();
		~Game();
		Game(const Game& g) = delete;
		Game& operator=(const Game& g) = delete;

		void create_main_menu();
		void create_settings_menu();

		void play_function(Ui* ui);
		void settings_function(Ui* ui);
		void quit_function(Ui* ui);

		void previous_menu_function(Ui* ui);
		void slider_sound_function(Ui* ui);
		void slider_music_function(Ui* ui);

		void texttoggle_full_screen_function(Ui* ui);
		void texttoggle_windowed_function(Ui* ui);

		void push_state(GameState* state);
		void pop_state(); //TODO : doit retourner un GameState* ??
		GameState* get_state();

		void handle_events();
		void draw();
		void update(Uint64 time_step);

		Menu* main_menu;
		Menu* settings_menu;
		Menu* load_menu;
		Menu* save_menu;

		bool game_running;
		sdl::Window window;

		Textbox* textbox;

	protected:

	private:
		sdl::SDL sdl;
		sdl::SDLImage sdl_img;
		sdl::SDLMixer sdl_mixer;
		sdl::SDLTTF sdl_ttf;

		sdl::Renderer renderer;
		sdl::GameController game_controller;
		std::stack<GameState*> states;

		static const int WINDOW_WIDTH = 1280; 
		static const int WINDOW_HEIGHT = 720;
};

