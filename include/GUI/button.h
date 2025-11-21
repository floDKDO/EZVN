#pragma once

#include "GUI/ui.h"
#include "image.h"

class Button : public Ui
{
	public:
		Button(const std::string path_normal, const std::string path_selected, const std::string path_clicked, const int x, const int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		void draw(sdl::Renderer& renderer) override;
		void update(Uint64 time_step) override;
		SDL_Rect get_rect() const override; 

	protected:

	private:
		Image normal_;
		Image selected_;
		Image clicked_;

		static const unsigned int INDEX_RECT_BUTTON = 0;
};

