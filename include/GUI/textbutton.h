#pragma once

#include "GUI/ui.h"
#include "text.h"

class TextButton : public Ui
{
	public:
		TextButton(const std::string text, const SDL_Color color_normal, const SDL_Color color_selected, const SDL_Color color_clicked, const int x, const int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		void draw(sdl::Renderer& renderer) override;
		void update(Uint64 time_step) override;
		SDL_Rect get_rect() const override;

	protected:

	private:
		Text text_;

		SDL_Color color_normal_;
		SDL_Color color_selected_;
		SDL_Color color_clicked_;

		static const unsigned int INDEX_RECT_TEXTBUTTON_ = 0;
};

