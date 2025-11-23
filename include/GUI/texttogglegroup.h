#pragma once

#include "GUI/checkablegroup.h"
#include "GUI/texttogglegroup.h"
#include "GUI/texttoggle.h"

template<size_t N>
class TextToggleGroup : public CheckableGroup
{
	public:
	TextToggleGroup(std::string top_text, std::vector<std::string> texts, SDL_Color color_unchecked, SDL_Color color_selected, SDL_Color color_checked, const int x, const int y, bool only_one_has_to_be_checked, sdl::Renderer& renderer, std::vector<std::function<void(Ui* ui)>> callback_functions);

	protected:

	private:
};


template<size_t N>
TextToggleGroup<N>::TextToggleGroup(std::string top_text, std::vector<std::string> texts, SDL_Color color_unchecked, SDL_Color color_selected, SDL_Color color_checked, const int x, const int y, bool only_one_has_to_be_checked, sdl::Renderer& renderer, std::vector<std::function<void(Ui* ui)>> callback_functions)
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
		checkables_.push_back(std::make_unique<TextToggle>(texts[i], color_unchecked, color_selected, color_checked, x, y + y_spacing_between_checkable_ * i, is_checked, renderer, callback_functions[i]));
		checkables_[i]->checkable_group_ = this;
	}
}