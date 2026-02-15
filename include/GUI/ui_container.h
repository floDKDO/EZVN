#pragma once

#include "GUI/ui_widget.h"
#include "text.h"

#include <vector>
#include <string>
#include <string_view>

class UiContainer : public Ui
{
	public:
		UiContainer(int x, int y);
		UiContainer(std::string_view title, int x, int y, sdl::Renderer& renderer);

		virtual ~UiContainer() = default;
		virtual void draw(sdl::Renderer& renderer) override;
		virtual void update() override;
		virtual std::vector<UiWidget*> get_navigation_nodes();
		virtual void add_ui_element(UiWidget* widget);

		void set_title(std::string_view title);

		std::vector<UiWidget*> ui_elements_;
		std::unique_ptr<Text> title_;

	protected:
		SDL_Rect frame_;

	private:
};

