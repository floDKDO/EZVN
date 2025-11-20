#include "GUI/checkable.h"
#include "GUI/checkablegroup.h"

#include <iostream>

Checkable::Checkable(bool is_checked, sdl::Renderer& renderer)
	: Ui(renderer), is_checked(is_checked), previous_checked(is_checked)
{

}

void Checkable::reverse_checked()
{
	this->previous_checked = this->is_checked; 
	this->is_checked = !this->is_checked;
}

void Checkable::change_checked(bool is_checked)
{
	this->previous_checked = this->is_checked;
	this->is_checked = is_checked;
}

void Checkable::on_pointer_up()
{ 
	Ui::on_pointer_up();
	if(this->checkable_group != nullptr)
	{
		this->checkable_group->on_click(this);
	}
}

void Checkable::on_pointer_up_hook_end()
{
	this->reverse_checked();
}

void Checkable::on_enter_released()
{
	Ui::on_enter_released();
	if(this->checkable_group != nullptr)
	{
		this->checkable_group->on_click(this);
	}
}

void Checkable::on_enter_released_hook_end()
{
	this->reverse_checked();
}