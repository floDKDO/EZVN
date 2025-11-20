#pragma once

#include "gamestate.h"
#include "menu.h"
#include "GUI/textbutton.h"
#include "textbox.h"
#include "GUI/inputfield.h"
#include "GUI/texttogglegroup.h"
#include "GUI/checkbox.h"
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

		void push_state(GameState* state);
		void pop_state(); //TODO : doit retourner un GameState* ??
		GameState* get_state();

		void handle_events();
		void draw();
		void update(Uint64 time_step);

	private:
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

		sdl::SDL sdl;
		sdl::SDLImage sdl_img;
		sdl::SDLMixer sdl_mixer;
		sdl::SDLTTF sdl_ttf;

	public:
		sdl::Window window;

	private:	
		sdl::Renderer renderer;
		sdl::GameController game_controller;

		std::unique_ptr<Menu> main_menu;
		std::unique_ptr<Menu> settings_menu;
		std::unique_ptr<Menu> load_menu;
		std::unique_ptr<Menu> save_menu;

	public:
		bool game_running;
		std::unique_ptr<Textbox> textbox;

	private:
		std::stack<GameState*> states;

		static const int WINDOW_WIDTH = 1280; 
		static const int WINDOW_HEIGHT = 720;
};

