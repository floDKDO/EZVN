#pragma once

#include "image.h"
#include "text.h"
#include "character.h"

class Textbox
{
	public:
		Textbox(sdl::Renderer& renderer);

		void change_textbox(std::string_view new_textbox_path, sdl::Renderer& renderer);
		void change_namebox(std::string_view new_namebox_path, sdl::Renderer& renderer);
		void change_namebox_text_color(SDL_Color new_namebox_text_color);
		void show_new_dialogue(const std::string_view new_dialogue, std::string speaker, bool in_skip_mode=false, bool wait_for_end_of_dialogue = true);

		Uint64 get_text_delay();

		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update();

		static const Uint64 minimum_time_;
		static const Uint64 maximum_time_;
		static const Uint64 base_delay_;

		Text text_;
		Image textbox_;

	private:
		std::string current_speaker_; 

		Image namebox_;
		Text text_name_box_;
		Image triangle_;

		bool is_first_dialogue_;
};

