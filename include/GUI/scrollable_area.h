#pragma once

#include "GUI/ui_widget.h"
#include "GUI/ui_group.h"
#include "GUI/scrollbar.h"

#include <memory>
#include <utility>

class ScrollableArea : public UiWidget
{
	public:
		ScrollableArea(int x, int y, int w, int h, sdl::Renderer& renderer);
		ScrollableArea(std::string_view title, int x, int y, int w, int h, sdl::Renderer& renderer);

		void callback_function(Ui* ui);
		void draw(sdl::Renderer& renderer) override;
		void update() override;
		std::vector<UiWidget*> get_navigation_nodes() override;
		void change_position(int x, int y) override;

		int get_scroll_offset(int ui_height);
		void add_ui_element(std::unique_ptr<UiWidget> widget);
		void add_ui_element(std::unique_ptr<UiGroup> ui_group);
		void add_text(std::string_view text); //TODO

	private:
		int get_max_y() const;

		SDL_Rect frame_;
		Scrollbar scrollbar_; 
		std::unique_ptr<Text> title_;

		std::vector<std::unique_ptr<Ui>> ui_elements_;
		std::vector<std::pair<UiWidget*, int/* init y position */>> ui_widgets_;
		int max_y_;
		sdl::Renderer& renderer_;
};

