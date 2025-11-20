#pragma once

#include "image.h"
#include "text.h"

class Textbox
{
	public:
		Textbox(SDL_Color text_color, sdl::Renderer& renderer);

		void show_new_dialogue(std::string new_dialogue, std::string speaker="");
		void draw(sdl::Renderer& renderer);
		void update(Uint64 time_step);

		std::string current_speaker; //TODO : à sûrement remplacer par Character
		Image namebox;
		Image textbox;
		Image triangle;
		Text text;
		Text text_name_box;

	protected:

	private:

};

