#pragma once

#include "GUI/ui_group.h"

class CheckableGroup : public UiGroup
{
	friend class Checkable;

	public:
		CheckableGroup(int x, int y, bool only_one_has_to_be_checked, Layout layout);
		CheckableGroup(std::string_view title, int x, int y, bool only_one_has_to_be_checked, Layout layout, sdl::Renderer& renderer);

		Checkable* add_ui_element(std::unique_ptr<Checkable> checkable);

	private:
		void uncheck_all_others(const Checkable* checkable_to_not_uncheck);
		void handle_only_one_has_to_be_checked(Checkable* checkable_to_not_uncheck);
		void on_press(Checkable* c);

		bool only_one_has_to_be_checked_;
};