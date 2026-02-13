#pragma once

#include "ui.h"
#include "scrollbar.h"

#include <memory>
#include <utility>

class ScrollableArea : public Ui
{
	public:
		ScrollableArea(int x, int y, int w, int h, sdl::Renderer& renderer);

		void callback_function(Ui* ui);
		void draw(sdl::Renderer& renderer) override;
		void update() override;
		SDL_Rect get_rect() const override;
		std::vector<Ui*> get_navigation_nodes() override;
		void add_ui_element(Ui* ui);

	private:
		SDL_Rect frame_;
		std::unique_ptr<Scrollbar> scrollbar_;
		std::vector<std::pair<Ui*, int /* init y position */ >> ui_elements_;
		sdl::Renderer& renderer_;
};

