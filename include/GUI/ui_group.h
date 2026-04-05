#pragma once

#include "GUI/ui_widget.h"
#include "text.h"

#include <vector>
#include <string_view>

class UiGroup : public Ui
{
	public:
		UiGroup(int x, int y);
		UiGroup(std::string_view title, int x, int y, sdl::Renderer& renderer);

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		std::vector<UiWidget*> get_navigation_nodes();
		UiWidget* add_ui_element(std::unique_ptr<UiWidget> widget);

		void set_title(std::string_view title);
		void set_center();

		std::vector<std::unique_ptr<UiWidget>> ui_elements_;
		std::unique_ptr<Text> title_;

	protected:
		SDL_Rect frame_;

	private:
};

