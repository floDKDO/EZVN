#pragma once

#include "GUI/ui.h"
#include "text.h"

class TextButton : public Ui
{
	public:
		TextButton(const std::string text, const int x, const int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function, const bool is_on_textbox=false); //TODO : aussi pour popupconfirm => renommer

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		SDL_Rect get_rect() const override;

	private:
		static const unsigned int index_rect_textbutton_;

	public:
		Text text_;
};

