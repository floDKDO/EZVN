#pragma once

#include "image.h"
#include "text.h"
#include "character.h"

class Textbox
{
	public:
		Textbox(sdl::Renderer& renderer);

		void set_initial_dialogue(const std::string new_dialogue, Character* speaker);
		void show_new_dialogue(const std::string new_dialogue, Character* speaker); 
		void show_new_dialogue(const std::string new_dialogue);

		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update();

		Text text_;
		Image textbox_;

	private:
		//Character current_speaker_;
		std::string current_speaker_; //TODO : à sûrement remplacer par Character

		Image namebox_;
		Text text_name_box_;
		Image triangle_;
};

