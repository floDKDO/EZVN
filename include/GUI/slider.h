#pragma once

#include "GUI/ui.h"
#include "image.h"
#include "text.h"

class Slider : public Ui
{
	public:
		Slider(const unsigned int min_value, const unsigned int max_value, const int x, const int y, const std::string_view text, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);
		Slider(const unsigned int min_value, const unsigned int max_value, const unsigned int current_value, const int x, const int y, const std::string_view text, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		bool is_mouse_on_handle(const int mouse_x, const int mouse_y) const;
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
		SDL_Rect get_rect() const override;

	private:
		static const unsigned int index_rect_container_;
		//static const unsigned int index_rect_handle_ = 1; //TODO : n'est normalement pas utile

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

