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

		void request_push_state(GameState* state);
		void request_pop_state();

		enum class Action
		{
			push,
			pop
		};

		struct RequestedAction
		{
			Action action;
			GameState* game_state;
		};

		std::stack<RequestedAction> requested_actions_;

		void handle_events();
		void draw();
		void update();

		void update_fps_count(const std::string_view fps) const;

		void create_character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, const std::string_view textbox_path="", const std::string_view namebox_path = "");
		void rename_character(const std::string_view character_variable, const std::string_view new_character_name);
		void show_character(const std::string_view character_variable, const std::string transform_name, const int zorder);
		void show_character(const std::string_view character_variable, const std::string transform_name);
		void show_character(const std::string_view character_variable, const int zorder = 0);
		void hide_character(const std::string_view character_variable);
		void add_new_dialogue(const std::string_view character_variable, const std::string_view dialogue);
		void add_new_dialogue(const std::string_view dialogue);
		void show_background(const std::string_view background_path);
		void show_background(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a);
		void hide_background();

	private:
		void push_state(GameState* state);
		void pop_state();
		GameState* get_state() const;

		void handle_requests();

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

