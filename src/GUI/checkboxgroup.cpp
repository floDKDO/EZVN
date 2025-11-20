#include "GUI/checkboxgroup.h"
#include "GUI/checkbox.h"

#include <iostream>

CheckboxGroup::CheckboxGroup(size_t number_of_checkboxes, std::string top_text, const std::string path_normal, const std::string path_selected, const std::string path_clicked, const std::string path_checked, const int x, const int y, bool only_one_has_to_be_checked, sdl::Renderer& renderer, std::vector<std::function<void(Ui* ui)>> callback_functions)
	: CheckableGroup(number_of_checkboxes, only_one_has_to_be_checked, top_text, x, y, renderer)
{
	SDL_assert(callback_functions.size() == number_of_checkboxes);

	bool is_checked = false;
	for(unsigned int i = 0; i < number_of_checkboxes; ++i)
	{
		if(only_one_has_to_be_checked && i == 0)
		{
			is_checked = true;
		}
		else
		{
			is_checked = false;
		}
		this->checkables.push_back(std::make_unique<Checkbox>(path_normal, path_selected, path_clicked, path_checked, x, y + this->y_spacing_between_checkable * i, is_checked, renderer, callback_functions[i]));
		this->checkables[i]->checkable_group = this;
	}
}