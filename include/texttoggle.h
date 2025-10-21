#pragma once

#include "ui.h"
#include "text.h"

class TextToggle : public Ui
{
	public:
		TextToggle(const std::string text, SDL_Color color_unchecked, SDL_Color color_selected, SDL_Color color_checked, const int x, const int y, bool is_checked, SDL_Renderer* renderer, void(*callback_function)(Ui* ui));

		void on_pointer_up() override; //<=> on click (l'action se lance quand le clic est relaché)
		void draw(SDL_Renderer* renderer) override;
		void update(Uint64& time_step) override;

	protected:

	private:
		Text text;

		SDL_Color color_unchecked;
		SDL_Color color_selected;
		SDL_Color color_checked;

		bool is_checked;
};

