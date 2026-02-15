#pragma once

#include "GUI/ui_widget.h"

//Pour l'instant, que de haut en bas

class Scrollbar : public UiWidget
{
	public:
		Scrollbar(int x, int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		bool is_mouse_on_handle(int mouse_x, int mouse_y) const;
		void disable_keyboard_focus();
		void handle_movement(int mouse_y);

		void on_pointer_down_hook_end(PointerEventData pointer_event_data) override;

		void on_drag(PointerEventData pointer_event_data) override;

		void on_up_pressed() override;
		void on_down_pressed() override;
		void on_left_pressed() override;
		void on_right_pressed() override;
		void on_enter_pressed_hook_end() override;

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		void change_position(int x, int y) override;

		float current_value_;
		float min_value_;
		float max_value_;

	protected:

	private:
		SDL_Rect container_;
		SDL_Rect container_outline_;
		SDL_Rect handle_;
		SDL_Rect handle_outline_;

		int delta_mouse_handle_y_; //y position of the mouse in comparison to the y position of the handle
};

