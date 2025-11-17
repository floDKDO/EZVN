#pragma once

#include "texttoggle.h"
#include "text.h"
#include <vector>

class TextToggleGroup : public Ui
{
	public:
		TextToggleGroup(size_t number, std::string top_text, std::vector<std::string>texts, SDL_Color color_unchecked, SDL_Color color_selected, SDL_Color color_checked, const int x, const int y, bool is_checked, SDL_Renderer* renderer, std::vector<std::function<void(Ui* ui)>> callback_functions);

		void on_pointer_up() override; //<=> on click (l'action se lance quand le clic est relaché)
		void on_enter_released() override;
		void on_input_released(const SDL_Event& e) override;
		void draw(SDL_Renderer* renderer) override;
		void update(Uint64 time_step) override;
		void handle_events(const SDL_Event& e) override;
		SDL_Rect get_rect() const override;
		std::vector<Ui*> get_navigation_nodes() override;

		std::vector<TextToggle*> toggles;

		bool only_one_has_to_be_checked; //TODO

	protected:

	private:
		unsigned int number;
		Text top_text;
};

