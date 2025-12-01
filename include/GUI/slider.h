#pragma once

#include "GUI/ui.h"
#include "image.h"
#include "text.h"

class Slider : public Ui
{
	public:
		Slider(const std::string path_bar, const std::string path_handle, unsigned int min_value, unsigned int max_value, const int x, const int y, std::string text, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		bool is_mouse_on_handle(int mouse_x, int mouse_y);
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

		unsigned int min_value_;
		unsigned int max_value_;
		unsigned int current_value_;

		bool is_selected_; //When we press Enter on a SELECTED Slider
		bool is_dragged_;
		//SDL_Rect handle_position; //TODO : n'est normalement pas utile
		int delta_mouse_handle_x_; //x position of the mouse in comparison to the x position of the handle

	protected:

	private:
		Image bar_;
		Image handle_;
		Text text_;

		static const unsigned int INDEX_RECT_BAR_ = 0;
		//static const unsigned int INDEX_RECT_HANDLE_ = 1; //TODO : n'est normalement pas utile
};

