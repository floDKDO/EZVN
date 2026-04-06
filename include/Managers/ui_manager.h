#pragma once

#include "GUI/ui_widget.h"
#include "Managers/audio_manager.h"

#include <vector>
#include <array>
#include <memory>

class UiManager
{
	public:
		UiManager(AudioManager& audio_manager, sdl::Renderer& renderer);

		void update_navigation_list(size_t ui_level);
		void reset_normal_ui();
		void reset_modal_ui();
		void reset();
		void register_element(Ui* ui);
		void set_elements();
		void show_pop_up(std::string_view text, std::function<void(Ui* ui)> callback_function);

		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update();

		bool is_mouse_on_widget(PointerEventData pointer_event_data);

		static const size_t modal_ui_; //= 1
		static const size_t normal_ui_; //= 0

		bool is_mouse_on_widget_;

	private:
		enum class Axis
		{
			X,
			Y
		};

		bool is_widget1_facing_widget2(SDL_Rect pos_widget1, SDL_Rect pos_widget2, Axis mode) const;
		bool is_candidate_closer(const UiWidget* widget, const UiWidget* candidate_widget, const UiWidget* current_best_widget, Axis mode) const;
		UiWidget* get_widget_facing(const UiWidget* widget, UiWidget* candidate_widget, UiWidget* current_best_widget, Axis mode) const;
		void assign_widget_on_moving(size_t ui_level) const;
		void select_new(UiWidget* widget);
		void unselect_previous(UiWidget* widget, PointerEventData pointer_event_data);

		bool is_mouse_on_specific_widget(UiWidget* widget, PointerEventData pointer_event_data);

		void on_input_pressed(const SDL_Event& e);
		void on_input_released(const SDL_Event& e);

		static constexpr size_t ui_levels_ = 2; //= 2

		//2 niveaux : [1] pour les UI modals, [0] pour les UI classiques
		std::array<std::vector<UiWidget*>, ui_levels_> navigation_list_;
		//std::array<std::vector<std::unique_ptr<Ui>>, ui_levels_> ui_elements_;

		std::array<std::vector<Ui*>, ui_levels_> ui_elements2_;

		UiWidget* previous_selected_;
		UiWidget* current_selected_;
		UiWidget* current_selected_normal_ui_;
		bool is_mouse_left_button_held_down_;
		bool is_pop_up_visible_;

		AudioManager& audio_manager_;
		sdl::Renderer& renderer_;

		Sound ui_press_sound_;
		Sound ui_select_sound_;

		Uint64 last_time_;
};

