#include "GUI/checkablegroup.h"
#include "GUI/checkboxgroup.h"

#include <iostream>


CheckableGroup::CheckableGroup(size_t number_of_checkables, bool only_one_has_to_be_checked, std::string top_text, int x, int y, sdl::Renderer& renderer)
	: Ui(renderer), y_spacing_text(-100), number_of_checkables(number_of_checkables), only_one_has_to_be_checked(only_one_has_to_be_checked), top_text(top_text, {255, 255, 255, 255}, "fonts/Aller_Rg.ttf", 50, x, y + this->y_spacing_text, renderer),
	y_spacing_between_checkable(200)
{
	//this->renderer = renderer;
}

void CheckableGroup::uncheck_all_others(Checkable* checkable_to_not_uncheck)
{
	for(std::unique_ptr<Checkable> const& c : checkables)
	{
		if(c.get() != checkable_to_not_uncheck)
		{
			c->change_checked(false);
		}
	}
}

void CheckableGroup::handle_only_one_has_to_be_checked(Checkable* checkable_to_not_uncheck)
{
	if(checkable_to_not_uncheck->previous_checked) //Cas clic sur un checkable déjà coché => ne pas le décocher dans ce cas
	{
		checkable_to_not_uncheck->change_checked(true);
	}
	this->uncheck_all_others(checkable_to_not_uncheck);
}

void CheckableGroup::on_click(Checkable* c)
{
	if(only_one_has_to_be_checked && (c->is_checked || c->previous_checked))
	{
		this->handle_only_one_has_to_be_checked(c);
	}
}

void CheckableGroup::draw(sdl::Renderer& renderer)
{
	for(std::unique_ptr<Checkable> const& c : checkables)
	{
		c->draw(renderer);
	}
	this->top_text.draw(renderer);
}

void CheckableGroup::update(Uint64 time_step)
{
	for(std::unique_ptr<Checkable> const& c : checkables)
	{
		c->update(time_step);
	}
	this->top_text.update(time_step);
}

void CheckableGroup::handle_events(const SDL_Event& e)
{
	for(std::unique_ptr<Checkable> const& c : checkables)
	{
		c->handle_events(e);
	}
}

std::vector<Ui*> CheckableGroup::get_navigation_nodes()
{
	std::vector<Ui*> vector;
	for(std::unique_ptr<Checkable> const& c : checkables)
	{
		vector.push_back(c.get());
	}
	return vector;
}