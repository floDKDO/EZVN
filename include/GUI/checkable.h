#pragma once

#include "GUI/ui.h"

class Checkable : public Ui
{
	friend class CheckableGroup;

	public:
		virtual ~Checkable() = default;
		virtual void reverse_checked();
		virtual void change_checked(bool is_checked);

		virtual void on_pointer_up();
		virtual void on_pointer_up_hook_end();

		virtual void on_enter_released();
		virtual void on_enter_released_hook_end();

		bool is_checked;
		bool previous_checked;
		CheckableGroup* checkable_group = nullptr;

	protected:
		Checkable(bool is_checked, sdl::Renderer& renderer);

	private:

};

