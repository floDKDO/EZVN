#pragma once

#include "GUI/ui.h"
#include "RAII_SDL2/chunk.h"
#include "audiomanager.h"

#include <vector>
#include <memory>

class UiManager
{
	public:
		UiManager(AudioManager& audio_manager);

	private:
		enum class Axis
		{
			X_AXIS,
			Y_AXIS
		};

	public:

		void add_element(std::unique_ptr<Ui>&& ui);
		void set_elements();
		bool is_ui1_facing_ui2(const SDL_Rect pos_ui1, const SDL_Rect pos_ui2, const Axis mode) const;
		bool is_candidate_closer(const Ui* const ui, const Ui* const candidate, const Ui* const current_best, const Axis mode) const;
		Ui* get_ui_facing(Ui* ui, Ui* candidate_ui, Ui* current_best_ui, const Axis mode) const;
		void assign_ui_on_moving() const;
		void select_new(Ui* ui);
		void unselect_previous(Ui* ui, PointerEventData pointer_event_data);

		bool is_mouse_on_ui(PointerEventData pointer_event_data);
		bool is_mouse_on_specific_ui(Ui* ui, PointerEventData pointer_event_data);

		void on_input_pressed(const SDL_Event& e);
		void on_input_released(const SDL_Event& e);

		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update();

		bool is_mouse_on_ui_;

		std::vector<Ui*> navigation_list_;
		std::vector<std::unique_ptr<Ui>> ui_elements_;

	private:
		Ui* previous_selected_;
		Ui* current_selected_;
		bool is_mouse_left_button_held_down_;
		//bool is_pop_up_visible_;

		AudioManager& audio_manager_;

		Sound click_sound_;
		Sound select_sound_;

		Uint64 last_time_;
};

