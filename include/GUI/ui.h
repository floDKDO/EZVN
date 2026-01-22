#pragma once

#include "pointereventdata.h"
#include "RAII_SDL2/renderer.h"

#include <functional>
#include <vector>

enum class State
{
	NORMAL, 
	SELECTED,
	PRESSED
};

class Ui
{
	public:
		//nécessaires pour les push_back de Buttons dans ButtonGroup
		Ui(const Ui& ui) = delete;
		Ui(Ui&& ui) = default;
		Ui& operator=(const Ui& ui) = delete;
		Ui& operator=(Ui&& ui) = default;

		virtual ~Ui() = default;

		virtual void on_pointer_up(PointerEventData pointer_event_data); 
		virtual void on_pointer_up_hook_end([[maybe_unused]] PointerEventData pointer_event_data) { }

		virtual void on_pointer_down(PointerEventData pointer_event_data);
		virtual void on_pointer_down_hook_end([[maybe_unused]] PointerEventData pointer_event_data) { }

		virtual void on_pointer_enter(PointerEventData pointer_event_data);
		virtual void on_pointer_enter_hook_end([[maybe_unused]] PointerEventData pointer_event_data) { }

		virtual void on_pointer_exit(PointerEventData pointer_event_data);
		virtual void on_pointer_exit_hook_end([[maybe_unused]] PointerEventData pointer_event_data) { }

		virtual void on_drag([[maybe_unused]] PointerEventData pointer_event_data){ }

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

		virtual void on_backspace_pressed() {}
		virtual void on_delete_pressed() {}


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

		virtual void on_typing([[maybe_unused]] const std::string_view text) { }


		virtual void draw(sdl::Renderer& renderer) = 0;
		virtual void update() {}; 

		virtual std::vector<Ui*> get_navigation_nodes();
		virtual SDL_Rect get_rect() const { return {0, 0, 0, 0}; }; 

		Ui* select_on_up_;
		Ui* select_on_down_;
		Ui* select_on_left_;
		Ui* select_on_right_;

		State state_;

		bool has_keyboard_focus_;
		bool mouse_entered_; 
		bool mouse_was_on_ui_before_drag_;
		bool wants_text_input_;
		bool pointer_on_ui_when_pointer_up_;
		bool callback_called_when_pointer_up_;

	protected:
		Ui(sdl::Renderer& renderer);

		Uint64 last_time_;

		sdl::Renderer& renderer_; //do not own it !!

		std::function<void(Ui* ui)> callback_function_;
};

