#pragma once

#include "constants.h"
#include "game_state.h"
#include "Managers/ui_manager.h"
#include "RAII_SDL2/window.h"
#include "RAII_SDL2/renderer.h"
#include "RAII_SDL2/game_controller.h"
#include "RAII_SDL2/sdl.h"
#include "RAII_SDL2/sdl_mixer.h"
#include "RAII_SDL2/sdl_img.h"
#include "RAII_SDL2/sdl_ttf.h"
#include "Managers/audio_manager.h"
#include "script.h"

#include <stack>

class InGame;

class Game
{
	public:
		Game();

		void init_game_states();
		void run();

		void request_push_state(std::string_view unique_id);
		void request_pop_state();
		void quit_game();

		void handle_events();
		void draw();
		void update();

		void update_fps_count(std::string_view fps) const;

		void create_composite_image(std::string_view character_variable, std::string_view composite_image_name, int w, int h, std::initializer_list<ImageInfo> images_info);
		void create_character(std::string_view character_variable, std::string_view character_name, Color namebox_text_color = constants::namebox_text_color_, std::string_view textbox_path = "", std::string_view namebox_path = "");
		void rename_character(std::string_view character_variable, std::string_view new_character_name);

		//std::pair<std::string_view, std::string_view> => first = character_variable, second = composite_image_name
		void show_character(std::pair<std::string_view, std::string_view> character_info, std::string transform_name, unsigned int zorder);
		void show_character(std::string_view character_variable, std::string_view transform_name, unsigned int zorder);
		void show_character(std::pair<std::string_view, std::string_view> character_info, std::string transform_name);
		void show_character(std::string_view character_variable, std::string_view transform_name);
		void show_character(std::pair<std::string_view, std::string_view> character_info, unsigned int zorder);
		void show_character(std::string_view character_variable, unsigned int zorder);
		void show_character(std::pair<std::string_view, std::string_view> character_info);
		void show_character(std::string_view character_variable);

		void hide_character(std::string_view character_variable);

		void show_dialogue(std::string_view character_variable, std::string_view dialogue);
		void show_dialogue(std::string_view dialogue);

		void show_background(std::string_view background_path);
		void show_background(Color color);

		void hide_background();

		void play_sound(std::string_view sound_path, int channel = 0, int fadein_length = 0, int fadeout_length = 0, float volume_multiplier = 1.0, bool loop = false);
		void stop_sound(int channel = -1, int fadeout_length = 0);

		void play_music(std::string_view music_path, int fadein_length=0, int fadeout_length=0, float volume_multiplier = 1.0, bool loop = true);
		void stop_music(int fadeout_length=0);

		void autofocus_enable();
		void autofocus_disable();

		void autozorder_enable();
		void autozorder_disable();

		void change_textbox(std::string_view character_variable, std::string_view textbox_path);
		void change_namebox(std::string_view character_variable, std::string_view namebox_path);
		void change_namebox_text_color(std::string_view character_variable, Color namebox_text_color);

		void move_textbox(std::string_view where);

	private:
		enum class Action
		{
			PUSH,
			POP
		};

		struct RequestedAction
		{
			Action action_;
			std::string_view unique_id_;
		};

		std::stack<RequestedAction> requested_actions_;

		void push_state(GameState* state);
		void pop_state();
		GameState* get_current_state() const;
		InGame* get_ingame_state() const;

		void handle_requests();

		sdl::SDL sdl_;
		sdl::SDLImage sdl_img_;
		sdl::SDLMixer sdl_mixer_;
		sdl::SDLTTF sdl_ttf_;

	public:
		sdl::Window window_;
		AudioManager audio_manager_;

	private:
		sdl::Renderer renderer_;
		sdl::GameController game_controller_;
		sdl::Surface window_icon_;
		std::unordered_map<std::string_view /* unique_id_ of the state */, std::unique_ptr<GameState>> game_states_map_; 
		bool window_is_open_;
		std::stack<GameState*> game_states_stack_;

	public: 
		Script script_;
};

