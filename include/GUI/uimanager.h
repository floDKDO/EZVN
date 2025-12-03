#pragma once

#include "GUI/ui.h"

#include <vector>
#include <memory>

class UiManager
{
	public:
		explicit UiManager(std::vector<std::unique_ptr<Ui>> ui_elements);

	private:
		enum class Axis
		{
			X_AXIS,
			Y_AXIS
		};

	public:

		bool is_ui1_facing_ui2(const SDL_Rect pos_ui1, const SDL_Rect pos_ui2, const Axis mode) const;
		bool is_candidate_closer(const Ui* const ui, const Ui* const candidate, const Ui* const current_best, const Axis mode) const;
		void get_ui_facing(Ui* ui, Ui* candidate_ui, Ui*& current_best_ui, const Axis mode) const;
		void assign_ui_on_moving() const;
		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update();

		std::vector<Ui*> navigation_list_;

	private:
		std::vector<std::unique_ptr<Ui>> ui_elements_;

		Ui* previous_selected_;
		Ui* current_selected_;
};

