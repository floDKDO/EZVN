#pragma once

#include "GUI/checkablegroup.h"

class CheckboxGroup : public CheckableGroup
{
	public:
		CheckboxGroup(size_t number_of_checkboxes, std::string top_text, const std::string path_normal, const std::string path_selected, const std::string path_clicked, const std::string path_checked, const int x, const int y, bool only_one_has_to_be_checked, sdl::Renderer& renderer, std::vector<std::function<void(Ui* ui)>> callback_functions);

	protected:

	private:	
};

