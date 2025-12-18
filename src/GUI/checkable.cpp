#include "GUI/checkable.h"
#include "GUI/checkablegroup.h"

#include <iostream>

Checkable::Checkable(const bool is_checked, sdl::Renderer& renderer)
	: Ui(renderer), is_checked_(is_checked), previous_checked_(is_checked), checkable_group_(nullptr)
{}

void Checkable::reverse_checked()
{
	previous_checked_ = is_checked_; 
	is_checked_ = !is_checked_;
}

void Checkable::change_checked(bool is_checked)
{
	previous_checked_ = is_checked_;
	is_checked_ = is_checked;
}

void Checkable::on_pointer_up_hook_end(PointerEventData pointer_event_data)
{
	(void)pointer_event_data;
	reverse_checked();
	if(checkable_group_ != nullptr)
	{
		checkable_group_->on_click(this);
	}
}

void Checkable::on_enter_released_hook_end()
{
	reverse_checked();
	if(checkable_group_ != nullptr)
	{
		checkable_group_->on_click(this);
	}
}