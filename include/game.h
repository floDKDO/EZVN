#pragma once

#include "gamestate.h"
#include "GUI/uimanager.h"
#include "RAII_SDL2/window.h"
#include "RAII_SDL2/renderer.h"
#include "RAII_SDL2/gamecontroller.h"
#include "RAII_SDL2/sdl.h"
#include "RAII_SDL2/sdlmixer.h"
#include "RAII_SDL2/sdlimg.h"
#include "RAII_SDL2/sdlttf.h"
#include "transform.h"

#include <stack>

class Game
{
	public:
		Game();

		void run();

		void push_state(GameState* state);
		void pop_state(); 
		GameState* get_state() const;

		void handle_events();
		void draw();
		void update();

		void update_fps_count(const std::string fps) const;

		void create_character(const std::string character_name, const std::string character_path);
		void show_character(unsigned int line_number, const std::string character_name, const TransformName transform_name);
		void show_character(unsigned int line_number, const std::string character_name);
		void hide_character(unsigned int line_number, const std::string character_name);
		void add_new_dialogue(unsigned int line_number, const std::string character_name, const std::string dialogue);
		void add_new_dialogue(unsigned int line_number, const std::string dialogue);

	private:
		//void create_main_menu();
		//void create_settings_menu();

		void play_function(Ui* ui);
		void settings_function(Ui* ui);

		void confirmationpopup_quit_function(Ui* ui);

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

		sdl::Surface window_icon_;

		UiManager ui_manager_;

	public:
		std::unique_ptr<GameState> main_menu_;
		std::unique_ptr<GameState> settings_menu_;
		std::unique_ptr<GameState> load_menu_;
		std::unique_ptr<GameState> save_menu_;
		std::unique_ptr<GameState> in_game_;

	public:
		bool game_running_;

	private:
		std::stack<GameState*> states_;
};

