#pragma once

#include "GUI/ui.h"

#include <vector>
#include <memory>

class UiManager
{
	public:
		UiManager(std::vector<std::unique_ptr<Ui>> ui_elements, Ui* ui_selected);

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
		void draw(sdl::Renderer& renderer);
		void update(Uint64 time_step);

		Ui* previous_selected_;
		Ui* current_selected_;

		std::vector<std::unique_ptr<Ui>> ui_elements_;
		std::vector<Ui*> navigation_list_;

	protected:

	private:
};

