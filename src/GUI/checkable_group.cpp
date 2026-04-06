#include "GUI/checkable_group.h"
#include "GUI/checkable.h"

CheckableGroup::CheckableGroup(int x, int y, bool only_one_has_to_be_checked, Layout layout)
	: UiGroup(x, y, layout), only_one_has_to_be_checked_(only_one_has_to_be_checked)
{}

CheckableGroup::CheckableGroup(std::string_view title, int x, int y, bool only_one_has_to_be_checked, Layout layout, sdl::Renderer& renderer)
	: UiGroup(title, x, y, layout, renderer), only_one_has_to_be_checked_(only_one_has_to_be_checked)
{}

Checkable* CheckableGroup::add_ui_element(std::unique_ptr<Checkable> checkable)
{
	if(Checkable* c = dynamic_cast<Checkable*>(checkable.get()); c != nullptr)
	{
		c->checkable_group_ = this;
	}
	return dynamic_cast<Checkable*>(UiGroup::add_ui_element(std::move(checkable)));
}

void CheckableGroup::uncheck_all_others(const Checkable* checkable_to_not_uncheck)
{
	for(std::unique_ptr<UiWidget>& ui_widget : ui_elements_)
	{
		Checkable* c = dynamic_cast<Checkable*>(ui_widget.get());
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