#pragma once

#include "GUI/checkablegroup.h"
#include "GUI/checkboxgroup.h"
#include "GUI/checkbox.h"
#include "constants.h"

template<size_t N>
class CheckboxGroup : public CheckableGroup
{
	public:
		CheckboxGroup(std::string_view top_text, std::vector<std::string> texts, int x, int y, bool only_one_has_to_be_checked, sdl::Renderer& renderer, std::vector<std::function<void(Ui* ui)>> callback_functions);
};


template<size_t N>
CheckboxGroup<N>::CheckboxGroup(std::string_view top_text, std::vector<std::string> texts, int x, int y, bool only_one_has_to_be_checked, sdl::Renderer& renderer, std::vector<std::function<void(Ui* ui)>> callback_functions)
	: CheckableGroup(N, only_one_has_to_be_checked, top_text, x, y, renderer)
{
	SDL_assert(callback_functions.size() == N);

	bool is_checked = false;
	for(unsigned int i = 0; i < N; ++i)
	{
		if(only_one_has_to_be_checked && i == 0)
		{
			is_checked = true;
		}
		else
		{
			is_checked = false;
		}
		checkables_.push_back(std::make_unique<Checkbox>(texts[i], x, y + constants::checkboxgroup_y_spacing_ * i, is_checked, renderer, callback_functions[i]));
		checkables_[i]->checkable_group_ = this;
	}
	top_text_.position_.x += (checkables_[0]->get_rect().w - top_text_.position_.w) / 2;
}