#include "texttogglegroup.h"

#include <iostream>

//TODO : pourquoi pas renommer cette classe en UIContainer qui contient un vector de Ui*

TextToggleGroup::TextToggleGroup(size_t number_of_toggles, std::string top_text, std::vector<std::string> texts, SDL_Color color_unchecked, SDL_Color color_selected, SDL_Color color_checked, const int x, const int y, bool only_one_has_to_be_checked, SDL_Renderer* renderer, std::vector<std::function<void(Ui* ui)>> callback_functions)
	: number_of_toggles(number_of_toggles), top_text(top_text, {255, 255, 255, 255}, "fonts/Aller_Rg.ttf", 50, x, y - 100, renderer), only_one_has_to_be_checked(only_one_has_to_be_checked) 
{
	SDL_assert(texts.size() == number_of_toggles && callback_functions.size() == number_of_toggles);

	this->renderer = renderer;
	
	bool is_checked = false;
	for(unsigned int i = 0; i < number_of_toggles; ++i)
	{
		if(only_one_has_to_be_checked && i == 0)
		{
			is_checked = true;
		}
		else
		{
			is_checked = false;
		}
		this->toggles.push_back(std::make_unique<TextToggle>(texts[i], color_unchecked, color_selected, color_checked, x, y + 100 * i, is_checked, renderer, callback_functions[i]));
		this->toggles[i]->texttoggle_group = this;
	}
}

void TextToggleGroup::uncheck_all_others(TextToggle* toggle_to_not_uncheck)
{
	for(std::unique_ptr<TextToggle> const& t : toggles)
	{
		if(t.get() != toggle_to_not_uncheck)
		{
			t->is_checked = false;
		}
	}
}

void TextToggleGroup::handle_only_one_has_to_be_checked(TextToggle* toggle_to_not_uncheck) 
{
	if(toggle_to_not_uncheck->previous_checked) //Cas clic sur un toggle déjà coché => ne pas le décocher dans ce cas
	{
		toggle_to_not_uncheck->is_checked = true;
	}
	this->uncheck_all_others(toggle_to_not_uncheck);
}

void TextToggleGroup::on_click(TextToggle* t)
{
	if(only_one_has_to_be_checked && (t->is_checked || t->previous_checked))
	{
		this->handle_only_one_has_to_be_checked(t);
	}
}

void TextToggleGroup::draw(SDL_Renderer* renderer)
{
	for(std::unique_ptr<TextToggle> const& t : toggles)
	{
		t->draw(renderer);
	}
	this->top_text.draw(renderer);
}

void TextToggleGroup::update(Uint64 time_step)
{
	for(std::unique_ptr<TextToggle> const& t : toggles)
	{
		t->update(time_step);
	}
	this->top_text.update(time_step);
}

void TextToggleGroup::handle_events(const SDL_Event& e)
{
	for(std::unique_ptr<TextToggle> const& t : toggles)
	{
		t->handle_events(e);
	}
}

std::vector<Ui*> TextToggleGroup::get_navigation_nodes()
{
	std::vector<Ui*> vector;
	for(std::unique_ptr<TextToggle> const& t : toggles)
	{
		vector.push_back(t.get());
	}
	return vector;
}