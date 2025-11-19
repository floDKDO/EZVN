#include "checkboxgroup.h"
#include "checkbox.h"

#include <iostream>

CheckboxGroup::CheckboxGroup(size_t number_of_toggles, std::string text, const std::string path_normal, const std::string path_selected, const std::string path_clicked, const std::string path_checked, const int x, const int y, bool only_one_has_to_be_checked, SDL_Renderer* renderer, std::vector<std::function<void(Ui* ui)>> callback_functions)
	: CheckableGroup(number_of_toggles, only_one_has_to_be_checked, text, x, y - 100, renderer)
{
	std::cout << "Constructeur de la classe CheckBoxGroup\n";

	SDL_assert(callback_functions.size() == number_of_toggles);

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
		this->toggles.push_back(std::make_unique<Checkbox>(path_normal, path_selected, path_clicked, path_checked, x, y + 200 * i, is_checked, renderer, callback_functions[i]));
		this->toggles[i]->checkable_group = this;
	}
}