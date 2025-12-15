#pragma once

#include "image.h"
#include "text.h"
#include "character.h"

class Textbox
{
	public:
		Textbox(sdl::Renderer& renderer);

		void set_initial_dialogue(const std::string_view new_dialogue, Character* speaker);
		void show_new_dialogue(const std::string_view new_dialogue, Character* speaker, bool in_skip_mode=false, bool wait_for_end_of_dialogue = true);
		void show_new_dialogue(const std::string_view new_dialogue);

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
		//Character current_speaker_;
		std::string current_speaker_; //TODO : à sûrement remplacer par Character

		Image namebox_;
		Text text_name_box_;
		Image triangle_;
};

