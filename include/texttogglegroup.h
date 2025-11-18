#pragma once

#include "texttoggle.h"
#include "text.h"
#include <vector>

class TextToggleGroup : public Ui
{
	public:
		TextToggleGroup(size_t number_of_toggles, std::string top_text, std::vector<std::string> texts, SDL_Color color_unchecked, SDL_Color color_selected, SDL_Color color_checked, const int x, const int y, bool only_one_has_to_be_checked, SDL_Renderer* renderer, std::vector<std::function<void(Ui* ui)>> callback_functions);

		void uncheck_all_others(TextToggle* toggle_to_not_uncheck);
		void handle_only_one_has_to_be_checked(TextToggle* toggle_to_not_uncheck);
		void on_click(TextToggle* t);

		void draw(SDL_Renderer* renderer) override;
		void update(Uint64 time_step) override;
		void handle_events(const SDL_Event& e) override;
		std::vector<Ui*> get_navigation_nodes() override;

		std::vector<std::unique_ptr<TextToggle>> toggles;

	protected:

	private:
		size_t number_of_toggles;
		bool only_one_has_to_be_checked;
		Text top_text;
};

