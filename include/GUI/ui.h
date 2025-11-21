#pragma once

#include "sound.h"
#include "RAII_SDL2/renderer.h"

#include <functional>
#include <vector>

enum class State
{
	NORMAL, 
	CLICKED, 
	SELECTED
};

class Ui
{
	public:
		virtual ~Ui() = default;

		virtual void on_pointer_up(); //<=> on click (l'action se lance quand le clic est relaché)
		virtual void on_pointer_up_hook_end() {}

		virtual void on_pointer_down();
		virtual void on_pointer_down_hook_end() {}

		virtual void on_pointer_enter();
		virtual void on_pointer_enter_hook_end() {}

		virtual void on_pointer_exit();
		virtual void on_pointer_exit_hook_end() {}


		virtual void on_up_pressed();
		virtual void on_up_pressed_hook_end() {}

		virtual void on_down_pressed();
		virtual void on_down_pressed_hook_end() {}

		virtual void on_left_pressed();
		virtual void on_left_pressed_hook_end() {}

		virtual void on_right_pressed();
		virtual void on_right_pressed_hook_end() {}

		virtual void on_enter_pressed();
		virtual void on_enter_pressed_hook_end() {}

		virtual void on_input_pressed(const SDL_Event& e);
		virtual void on_input_pressed_hook_end(const SDL_Event& e) { (void)e; }


		virtual void on_up_released();
		virtual void on_up_released_hook_end() {}

		virtual void on_down_released();
		virtual void on_down_released_hook_end() {}

		virtual void on_left_released();
		virtual void on_left_released_hook_end() {}

		virtual void on_right_released();
		virtual void on_right_released_hook_end() {}

		virtual void on_enter_released();
		virtual void on_enter_released_hook_end() {}

		virtual void on_input_released(const SDL_Event& e);
		virtual void on_input_released_hook_end(const SDL_Event& e) { (void)e; }


		virtual void handle_events(const SDL_Event& e);
		virtual void handle_events_hook_end(const SDL_Event& e) { (void)e; }

		virtual void draw(sdl::Renderer& renderer) = 0;
		virtual void update(Uint64 time_step) = 0;

		void get_logical_mouse_position(int* logical_mouse_x, int* logical_mouse_y) const;
		int is_mouse_on_ui();
		static const int MOUSE_NOT_ON_ANY_UI = -1;

		virtual std::vector<Ui*> get_navigation_nodes();
		virtual SDL_Rect get_rect() const { return {0, 0, 0, 0}; }; 

		Ui* select_on_up_;
		Ui* select_on_down_;
		Ui* select_on_left_;
		Ui* select_on_right_;

		State state_;

		Uint64 last_time_; //TODO : trouver un meilleur nom

		static bool lock_; 

	protected:
		Ui(sdl::Renderer& renderer);

		//TODO : stocker en "variables globales" select_sound, click_sound et une police par défaut ?
		Sound select_sound_;
		Sound click_sound_;
		bool is_selected_sound_played_;
		bool pointer_on_ui_when_pointer_up_; 
		sdl::Renderer& renderer_; //do not own it !!

		std::function<void(Ui* ui)> callback_function_;


	private:
		void select_new(Ui* ui);

};

