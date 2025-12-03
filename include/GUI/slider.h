#pragma once

#include "GUI/ui.h"
#include "image.h"
#include "text.h"

class Slider : public Ui
{
	public:
		Slider(const unsigned int min_value, const unsigned int max_value, const int x, const int y, const std::string text, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		bool is_mouse_on_handle(const int mouse_x, const int mouse_y) const;
		void handle_movement();
		void unselect();

		void on_pointer_up() override; //<=> on click (l'action se lance quand le clic est relaché)
		void on_pointer_down_hook_end() override;
		void on_pointer_enter_hook_end() override;
		void on_pointer_exit_hook_end() override;

		void on_up_pressed() override;
		void on_down_pressed() override;
		void on_left_pressed() override;
		void on_right_pressed() override;
		void on_enter_pressed_hook_end() override;

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		void handle_events_hook_end(const SDL_Event& e) override;
		SDL_Rect get_rect() const override;

	protected:

	private:
		Image container_;
		Image handle_;
		Text text_;

		unsigned int min_value_;
		unsigned int max_value_;

	public:
		unsigned int current_value_;

	private:
		bool is_selected_; //When we press Enter on a SELECTED Slider
		bool is_dragged_;
		//SDL_Rect handle_position; //TODO : n'est normalement pas utile
		int delta_mouse_handle_x_; //x position of the mouse in comparison to the x position of the handle

		static const unsigned int index_rect_container_;
		//static const unsigned int index_rect_handle_ = 1; //TODO : n'est normalement pas utile
};

