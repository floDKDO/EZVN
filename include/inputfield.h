#pragma once

#include "ui.h"
#include "text.h"
#include "image.h"

class Inputfield : public Ui
{
	public:
		Inputfield(const std::string path, const SDL_Color color_normal, unsigned int character_limit, const int x, const int y, SDL_Renderer* renderer, void(*callback_function)(Ui* ui));

		void on_pointer_up() override; //<=> on click (l'action se lance quand le clic est relach�)
		//TODO : abstraire le fait que �a soit une touche du clavier ou la manette dans les if
		void on_key_pressed(const SDL_Event& e) override;
		void on_key_released(const SDL_Event& e) override;
		void handle_events(const SDL_Event& e) override;
		void draw(SDL_Renderer* renderer) override;
		void update(Uint64& time_step) override;

		void on_input(const SDL_Event& e);
		void set_character_limit(unsigned int character_limit);

		Text text;

	protected:

	private:
		Image normal;
		SDL_Color color_normal;

		unsigned int character_limit;

		Text text_caret;
		Text text_placeholder;

		bool is_editing;
		bool is_writing;
		
};

