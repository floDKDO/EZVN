#pragma once

#include "ui.h"
#include "text.h"

class TextButton : public Ui
{
	public:
		TextButton(const std::string text, const SDL_Color color_normal, const SDL_Color color_selected, const SDL_Color color_clicked, const int x, const int y, SDL_Renderer* renderer, std::function<void(Ui* ui)> callback_function);

		void draw(SDL_Renderer* renderer) override;
		void update(Uint64 time_step) override;
		std::vector<SDL_Rect> get_bounds() const override;

	protected:

	private:
		Text text;

		SDL_Color color_normal;
		SDL_Color color_selected;
		SDL_Color color_clicked;
};

