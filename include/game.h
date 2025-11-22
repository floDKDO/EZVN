#pragma once

#include "gamestate.h"
#include "menu.h"
#include "textbox.h"
#include "RAII_SDL2/window.h"
#include "RAII_SDL2/renderer.h"
#include "RAII_SDL2/gamecontroller.h"
#include "RAII_SDL2/sdl.h"
#include "RAII_SDL2/sdlmixer.h"
#include "RAII_SDL2/sdlimg.h"
#include "RAII_SDL2/sdlttf.h"

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
		void slider_text_function(Ui* ui);

		void texttoggle_full_screen_function(Ui* ui);
		void texttoggle_windowed_function(Ui* ui);

		sdl::SDL sdl_;
		sdl::SDLImage sdl_img_;
		sdl::SDLMixer sdl_mixer_;
		sdl::SDLTTF sdl_ttf_;

	public:
		sdl::Window window_;

	private:	
		sdl::Renderer renderer_;
		sdl::GameController game_controller_;

		std::unique_ptr<Menu> main_menu_;
		std::unique_ptr<Menu> settings_menu_;
		std::unique_ptr<Menu> load_menu_;
		std::unique_ptr<Menu> save_menu_;

	public:
		bool game_running_;
		Textbox textbox_;

	private:
		std::stack<GameState*> states_;

		static const int WINDOW_WIDTH_ = 1280; 
		static const int WINDOW_HEIGHT_ = 720;
};

