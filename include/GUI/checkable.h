#pragma once

#include "GUI/ui.h"

class CheckableGroup;

class Checkable : public Ui
{
	public:
		virtual ~Checkable() = default;
		virtual void reverse_checked();
		virtual void change_checked(bool is_checked);

		void on_pointer_up_hook_end(PointerEventData pointer_event_data) override;
		void on_enter_released_hook_end() override;

		bool is_checked_;
		bool previous_checked_;
		CheckableGroup* checkable_group_;

	protected:
		Checkable(bool is_checked, sdl::Renderer& renderer);
};

