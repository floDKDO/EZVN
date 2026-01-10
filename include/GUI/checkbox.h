#pragma once

#include "GUI/checkable.h"
#include "image.h"
#include "text.h"

#include <string>

class Checkbox : public Checkable
{
	public:
		Checkbox(const std::string_view text, const int x, const int y, const bool is_checked, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		SDL_Rect get_rect() const override;

	private:
		static const unsigned int index_rect_checkbox_;

		Image normal_;
		Image selected_;
		Image pressed_;
		Image check_mark_; 

		Text text_;
};

