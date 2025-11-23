#pragma once

#include "image.h"
#include "text.h"

class Textbox
{
	public:
		Textbox(SDL_Color text_color, sdl::Renderer& renderer);

		void show_new_dialogue(std::string new_dialogue, std::string speaker="");

		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update(Uint64 time_step);

		std::string current_speaker_; //TODO : à sûrement remplacer par Character
		Image namebox_;
		Image textbox_;
		Image triangle_;
		Text text_;
		Text text_name_box_;

	protected:

	private:

};

