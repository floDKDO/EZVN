#pragma once

#include "GUI/ui_widget.h"
#include "GUI/ui_group.h"
#include "GUI/scrollbar.h"

#include <memory>
#include <utility>
#include <variant>

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
		void add_text(std::pair<std::unique_ptr<Text>, std::unique_ptr<Text>> text); //TODO

	private:
		int get_max_y() const;

		SDL_Rect frame_;
		Scrollbar scrollbar_; 
		std::unique_ptr<Text> title_;

		//using Dialogue = std::pair<std::unique_ptr<Text>, std::unique_ptr<Text>>;
		
		//ancien ui_elements_
		std::vector<std::variant<std::unique_ptr<Ui>, std::pair<std::unique_ptr<Text>, std::unique_ptr<Text>>>> elements_;

		//TODO : utiliser using pour simplifier les types

		//ancien ui_widgets_
		std::vector<std::pair<std::variant<UiWidget*, std::pair<Text*, Text*>>, int/*init y position*/>> elements_value_;

		int max_y_;
		sdl::Renderer& renderer_;
};

