#pragma once

#include "GUI/ui_widget.h"
#include "Managers/audio_manager.h"

#include <vector>
#include <memory>

class UiManager
{
	public:
		UiManager(AudioManager& audio_manager);

		void reset();
		void add_element(std::unique_ptr<Ui> ui);
		//Ui* get_last_element();
		void set_elements();

		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update();

		bool is_mouse_on_widget(PointerEventData pointer_event_data);

		bool is_mouse_on_widget_;

	private:
		enum class Axis
		{
			X,
			Y
		};

		bool is_widget1_facing_widget2(SDL_Rect pos_ui1, SDL_Rect pos_ui2, Axis mode) const;
		bool is_candidate_closer(const UiWidget* widget, const UiWidget* candidate_widget, const UiWidget* current_best_widget, Axis mode) const;
		UiWidget* get_widget_facing(const UiWidget* widget, UiWidget* candidate_widget, UiWidget* current_best_widget, Axis mode) const;
		void assign_widget_on_moving() const;
		void select_new(UiWidget* widget);
		void unselect_previous(UiWidget* widget, PointerEventData pointer_event_data);

		bool is_mouse_on_specific_widget(UiWidget* widget, PointerEventData pointer_event_data);

		void on_input_pressed(const SDL_Event& e);
		void on_input_released(const SDL_Event& e);

		std::vector<UiWidget*> navigation_list_;
		std::vector<std::unique_ptr<Ui>> ui_elements_;

		UiWidget* previous_selected_;
		UiWidget* current_selected_;
		bool is_mouse_left_button_held_down_;
		//bool is_pop_up_visible_;

		AudioManager& audio_manager_;

		Sound ui_press_sound_;
		Sound ui_select_sound_;

		Uint64 last_time_;
};

