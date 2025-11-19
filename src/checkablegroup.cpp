#include "checkablegroup.h"

#include "checkboxgroup.h"

#include <iostream>

//TODO : pourquoi pas renommer cette classe en UIContainer qui contient un vector de Ui*

CheckableGroup::CheckableGroup(size_t number_of_toggles, bool only_one_has_to_be_checked, std::string top_text, SDL_Renderer* renderer)
	: number_of_toggles(number_of_toggles), only_one_has_to_be_checked(only_one_has_to_be_checked), top_text(top_text, {255, 255, 255, 255}, "fonts/Aller_Rg.ttf", 50, 0, 0, renderer)
{
	std::cout << "Constructeur de la classe de base CheckableGroup\n";
}

CheckableGroup::~CheckableGroup()
{

}

void CheckableGroup::uncheck_all_others(Checkable* toggle_to_not_uncheck)
{
	for(std::unique_ptr<Checkable> const& t : toggles)
	{
		if(t.get() != toggle_to_not_uncheck)
		{
			t->is_checked = false;
		}
	}
}

void CheckableGroup::handle_only_one_has_to_be_checked(Checkable* toggle_to_not_uncheck)
{
	if(toggle_to_not_uncheck->previous_checked) //Cas clic sur un toggle déjà coché => ne pas le décocher dans ce cas
	{
		toggle_to_not_uncheck->is_checked = true;
	}
	this->uncheck_all_others(toggle_to_not_uncheck);
}

void CheckableGroup::on_click(Checkable* t)
{
	if(only_one_has_to_be_checked && (t->is_checked || t->previous_checked))
	{
		this->handle_only_one_has_to_be_checked(t);
	}
}

void CheckableGroup::draw(SDL_Renderer* renderer)
{
	for(std::unique_ptr<Checkable> const& t : toggles)
	{
		t->draw(renderer);
	}
	this->top_text.draw(renderer);
}

void CheckableGroup::update(Uint64 time_step)
{
	for(std::unique_ptr<Checkable> const& t : toggles)
	{
		t->update(time_step);
	}
	this->top_text.update(time_step);
}

void CheckableGroup::handle_events(const SDL_Event& e)
{
	for(std::unique_ptr<Checkable> const& t : toggles)
	{
		t->handle_events(e);
	}
}

std::vector<Ui*> CheckableGroup::get_navigation_nodes()
{
	std::vector<Ui*> vector;
	for(std::unique_ptr<Checkable> const& t : toggles)
	{
		vector.push_back(t.get());
	}
	return vector;
}