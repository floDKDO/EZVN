#include "GUI/checkablegroup.h"
#include "constants.h"

#include <iostream>

CheckableGroup::CheckableGroup(size_t number_of_checkables, bool only_one_has_to_be_checked, std::string_view top_text, int x, int y, sdl::Renderer& renderer)
	: Ui(renderer), 
	top_text_(top_text, constants::checkablegroup_text_color_, constants::checkablegroup_font_, constants::checkablegroup_text_size_, x, y + constants::checkablegroup_top_text_y_delta_, renderer),
    number_of_checkables_(number_of_checkables), selected_checkable_(nullptr), 
	only_one_has_to_be_checked_(only_one_has_to_be_checked)
{}

void CheckableGroup::uncheck_all_others(const Checkable* checkable_to_not_uncheck)
{
	for(const std::unique_ptr<Checkable>& c : checkables_)
	{
		if(c.get() != checkable_to_not_uncheck)
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

void CheckableGroup::draw(sdl::Renderer& renderer)
{
	for(const std::unique_ptr<Checkable>& c : checkables_)
	{
		c->draw(renderer);
	}
	top_text_.draw(renderer);
}

//TODO : utile ??
void CheckableGroup::update()
{
	for(const std::unique_ptr<Checkable>& c : checkables_)
	{
		c->update();
		if(c->state_ == State::SELECTED)
		{
			if(selected_checkable_ == nullptr || selected_checkable_ == c.get())
			{
				selected_checkable_ = c.get();
			}
			else
			{
				selected_checkable_->state_ = State::NORMAL;
			}
		}
	}
	selected_checkable_ = nullptr;

	top_text_.update();
}

//TODO : utile ??
/*void CheckableGroup::handle_events(const SDL_Event& e)
{
	for(const std::unique_ptr<Checkable>& c : checkables_)
	{
		c->handle_events(e);
	}
}*/

std::vector<Ui*> CheckableGroup::get_navigation_nodes()
{
	std::vector<Ui*> vector;
	for(const std::unique_ptr<Checkable>& c : checkables_)
	{
		vector.push_back(c.get());
	}
	return vector;
}