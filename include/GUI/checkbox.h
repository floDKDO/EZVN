#pragma once

#include "GUI/checkable.h"
#include "image.h"

class Checkbox : public Checkable
{
	public:
		Checkbox(const int x, const int y, bool is_checked, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		SDL_Rect get_rect() const override;

	protected:

	private:
		Image normal_;
		Image selected_;
		Image clicked_;
		Image check_mark_; 

		static const unsigned int INDEX_RECT_CHECKBOX_ = 0;
};

