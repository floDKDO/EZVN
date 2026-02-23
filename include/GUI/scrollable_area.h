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
		using DialoguePtr = std::pair<std::unique_ptr<Text>, std::unique_ptr<Text>>;
		using Dialogue = std::pair<Text*, Text*>;

		ScrollableArea(int x, int y, int w, int h, sdl::Renderer& renderer);
		ScrollableArea(std::string_view title, int x, int y, int w, int h, sdl::Renderer& renderer);

		void scroll_elements();
		void callback_function(Ui* ui);
		void draw(sdl::Renderer& renderer) override;
		void update() override;
		std::vector<UiWidget*> get_navigation_nodes() override;
		void change_position(int x, int y) override;

		int get_scroll_offset();
		void add_ui_element(std::unique_ptr<UiWidget> widget);
		void add_ui_element(std::unique_ptr<UiGroup> ui_group);
		void add_text(DialoguePtr text);
		void remove_last_text();
		Text* get_last_dialogue();
		int get_last_dialogue_initial_y_pos();

	private:
		int get_max_y() const;

		SDL_Rect frame_;
		Scrollbar scrollbar_; 
		std::unique_ptr<Text> title_;
		
		//TODO : utiliser using pour simplifier les types

		std::vector<std::variant<std::unique_ptr<Ui>, DialoguePtr>> elements_;
		std::vector<std::pair<std::variant<UiWidget*, Dialogue>, int/*init y position*/>> elements_value_;

		sdl::Renderer& renderer_;
};

