#pragma once

#include "GUI/checkable.h"
#include "text.h"

class TextToggle : public Checkable
{
	public:
		TextToggle(const std::string text, const int x, const int y, const bool is_checked, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		SDL_Rect get_rect() const override;

	private:
		static const unsigned int index_rect_texttoggle_;

		Text text_;
};

