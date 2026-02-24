#pragma once

#include "GUI/ui_widget.h"
#include "text.h"

class Slider : public UiWidget
{
	public:
		Slider(unsigned int min_value, unsigned int max_value, int x, int y, std::string_view text, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);
		Slider(unsigned int min_value, unsigned int max_value, unsigned int current_value, int x, int y, std::string_view text, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		void disable_keyboard_focus();
		void handle_movement(int mouse_x);

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

	private:
		SDL_Rect container_;
		SDL_Rect container_outline_;
		SDL_Rect handle_;
		SDL_Rect handle_outline_;
		Text text_;

		unsigned int min_value_;
		unsigned int max_value_;

	public:
		unsigned int current_value_;

	private:
		//SDL_Rect handle_position; //TODO : n'est normalement pas utile
		int delta_mouse_handle_x_; //x position of the mouse in comparison to the x position of the handle
};

