#pragma once

#include "checkablegroup.h"
#include "texttoggle.h"
#include "text.h"
#include <vector>

class TextToggleGroup : public CheckableGroup
{
	public:
		TextToggleGroup(size_t number_of_toggles, std::string top_text, std::vector<std::string> texts, SDL_Color color_unchecked, SDL_Color color_selected, SDL_Color color_checked, const int x, const int y, bool only_one_has_to_be_checked, SDL_Renderer* renderer, std::vector<std::function<void(Ui* ui)>> callback_functions);

	protected:

	private:
};

