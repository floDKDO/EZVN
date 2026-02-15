#include "GUI/checkable_group.h"
#include "constants.h"

#include <iostream>

CheckableGroup::CheckableGroup(std::string_view title, int x, int y, bool only_one_has_to_be_checked, sdl::Renderer& renderer)
	: UiContainer(title, x, y, renderer), only_one_has_to_be_checked_(only_one_has_to_be_checked)
{}

void CheckableGroup::add_ui_element(UiWidget* widget)
{
	Checkable* c = dynamic_cast<Checkable*>(widget);
	if(c != nullptr)
	{
		UiContainer::add_ui_element(widget);
		c->checkable_group_ = this;
	}
	else
	{
		std::cerr << "This UiWidget element is not checkable! Not added to the CheckableGroup!\n";
	}
}

void CheckableGroup::uncheck_all_others(const Checkable* checkable_to_not_uncheck)
{
	for(Ui* ui : ui_elements_)
	{
		Checkable* c = dynamic_cast<Checkable*>(ui);
		if(c != nullptr && c != checkable_to_not_uncheck)
		{
			c->change_checked(false);
		}
	}
}

void CheckableGroup::handle_only_one_has_to_be_checked(Checkable* checkable_to_not_uncheck)
{
	if(checkable_to_not_uncheck->previous_checked_) //Cas press sur un checkable déjà coché => ne pas le décocher dans ce cas
	{
		checkable_to_not_uncheck->change_checked(true);
	}
	uncheck_all_others(checkable_to_not_uncheck);
}

void CheckableGroup::on_press(Checkable* c)
{
	if(only_one_has_to_be_checked_ && (c->is_checked_ || c->previous_checked_))
	{
		handle_only_one_has_to_be_checked(c);
	}
}