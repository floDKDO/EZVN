#pragma once

#include "GUI/ui_container.h"
#include "GUI/checkable.h"
#include "text.h"

#include <memory>

class CheckableGroup : public UiContainer
{
	public:
		CheckableGroup(std::string_view title, int x, int y, bool only_one_has_to_be_checked, sdl::Renderer& renderer);

		void add_ui_element(UiWidget* widget) override;

		void uncheck_all_others(const Checkable* checkable_to_not_uncheck);
		void handle_only_one_has_to_be_checked(Checkable* checkable_to_not_uncheck);
		void on_press(Checkable* c);

	private:
		bool only_one_has_to_be_checked_;
};

