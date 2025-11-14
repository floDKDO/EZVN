#pragma once

#include "gamestate.h"
#include "button.h"
#include "slider.h"

#include <vector>

class Menu : public GameState
{
	public:
		Menu(std::vector<Ui*> ui, Ui* ui_selected);
		~Menu();
		Menu(const Menu& m) = delete;
		Menu& operator=(const Menu& m) = delete;

		enum class Axis
		{
			X_AXIS,
			Y_AXIS
		};

		bool is_candidate_closer(const Ui* const ui, const Ui* const candidate, const Ui* const current_best, const Axis mode);
		void get_ui_facing(Ui* ui, Ui* candidate_ui, Ui*& current_best_ui, const Axis mode);
		void assign_ui_on_moving();
		bool is_ui1_facing_ui2(const SDL_Rect pos_ui1, const SDL_Rect pos_ui2, const Axis mode);
		void handle_events(const SDL_Event& e);
		void draw(SDL_Renderer* renderer);
		void update(Uint64 time_step);

		Ui* previous_selected;
		Ui* current_selected;
		std::vector<Ui*> ui;

	protected:

	private:
};

