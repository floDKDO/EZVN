#pragma once

#include "GUI/checkablegroup.h"
#include "GUI/texttoggle.h"
#include "constants.h"

template<size_t N>
class TextToggleGroup : public CheckableGroup
{
	public:
		TextToggleGroup(const std::string_view top_text, const  std::vector<std::string_view> texts, const int x, const int y, const bool only_one_has_to_be_checked, sdl::Renderer& renderer, std::vector<std::function<void(Ui* ui)>> callback_functions);
};


template<size_t N>
TextToggleGroup<N>::TextToggleGroup(const std::string_view top_text, const std::vector<std::string_view> texts, const int x, const int y, const bool only_one_has_to_be_checked, sdl::Renderer& renderer, std::vector<std::function<void(Ui* ui)>> callback_functions)
	: CheckableGroup(N, only_one_has_to_be_checked, top_text, x, y, renderer)
{
	SDL_assert(texts.size() == N && callback_functions.size() == N);

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
		checkables_.push_back(std::make_unique<TextToggle>(texts[i], x, y + constants::texttogglegroup_y_spacing_ * i, is_checked, renderer, callback_functions[i]));
		checkables_[i]->checkable_group_ = this;
	}
	top_text_.position_.x += (checkables_[0]->get_rect().w - top_text_.position_.w) / 2;
}