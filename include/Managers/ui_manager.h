#pragma once

#include "GUI/ui.h"
#include "Managers/audio_manager.h"

#include <vector>
#include <memory>

class UiManager
{
	public:
		UiManager(AudioManager& audio_manager);

		void add_element(std::unique_ptr<Ui>&& ui);
		void set_elements();

		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update();

		bool is_mouse_on_ui(PointerEventData pointer_event_data);

		bool is_mouse_on_ui_;

	private:
		enum class Axis
		{
			X,
			Y
		};

		bool is_ui1_facing_ui2(SDL_Rect pos_ui1, SDL_Rect pos_ui2, Axis mode) const;
		bool is_candidate_closer(const Ui* ui, const Ui* candidate, const Ui* current_best, Axis mode) const;
		Ui* get_ui_facing(const Ui* ui, Ui* candidate_ui, Ui* current_best_ui, Axis mode) const;
		void assign_ui_on_moving() const;
		void select_new(Ui* ui);
		void unselect_previous(Ui* ui, PointerEventData pointer_event_data);

		bool is_mouse_on_specific_ui(Ui* ui, PointerEventData pointer_event_data);

		void on_input_pressed(const SDL_Event& e);
		void on_input_released(const SDL_Event& e);

		std::vector<Ui*> navigation_list_;
		std::vector<std::unique_ptr<Ui>> ui_elements_;

		Ui* previous_selected_;
		Ui* current_selected_;
		bool is_mouse_left_button_held_down_;
		//bool is_pop_up_visible_;

		AudioManager& audio_manager_;

		Sound ui_press_sound_;
		Sound ui_select_sound_;

		Uint64 last_time_;
};

