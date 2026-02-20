#pragma once

#include "GUI/ui_widget.h"
#include "text.h"

#include <vector>
#include <string>
#include <string_view>

class UiGroup : public Ui
{
	friend class Checkable;

	public:
		UiGroup(int x, int y);
		UiGroup(std::string_view title, int x, int y, sdl::Renderer& renderer);

		virtual ~UiGroup() = default;
		virtual void draw(sdl::Renderer& renderer) override;
		virtual void update() override;
		virtual std::vector<UiWidget*> get_navigation_nodes();
		virtual void add_ui_element(std::unique_ptr<UiWidget> widget);

		void set_title(std::string_view title);

		std::vector<std::unique_ptr<UiWidget>> ui_elements_;
		std::unique_ptr<Text> title_;
		bool only_one_has_to_be_checked_;

	protected:
		SDL_Rect frame_;

	private:
		void uncheck_all_others(const Checkable* checkable_to_not_uncheck);
		void handle_only_one_has_to_be_checked(Checkable* checkable_to_not_uncheck);
		void on_press(Checkable* c);
};

