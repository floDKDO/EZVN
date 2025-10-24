#pragma once

#include <vector>
#include "button.h"

class Menu
{
	public:
		Menu(std::vector<Ui*>& ui);
		~Menu();

		void assign_ui_on_moving();
		bool is_ui_facing(Ui* ui1, Ui* ui2, char mode);
		void handle_events(const SDL_Event& e);
		void draw(SDL_Renderer* renderer);
		void update(Uint64& time_step);

		Ui* previous_selected;
		Ui* current_selected;
		std::vector<Ui*> ui;

	protected:

	private:
};

