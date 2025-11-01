#pragma once

#include "ui.h"
#include "image.h"

//TODO : que gauche à droite pour l'instant
//TODO : que des nombres entiers pour l'instant
class Slider : public Ui
{
	public:
		Slider(const std::string path_bar, const std::string path_handle, unsigned int min_value, unsigned int max_value, const int x, const int y, SDL_Renderer* renderer, std::function<void(Ui* ui)> callback_function);

		bool is_mouse_on_handle(int mouse_x, int mouse_y);
		void on_pointer_up(bool pointer_on_ui_when_pointer_up) override; //<=> on click (l'action se lance quand le clic est relaché)
		void on_pointer_down() override;
		void on_pointer_enter() override;
		void on_pointer_exit() override;

		void on_left_pressed() override;
		void on_right_pressed() override;
		void on_enter_pressed() override;

		void draw(SDL_Renderer* renderer) override;
		void update(Uint64& time_step) override;

		unsigned int min_value;
		unsigned int max_value;
		unsigned int current_value;
		bool is_dragged;
		//SDL_Rect handle_position;
		int diff;

	protected:

	private:
		Image bar;
		Image handle;
};

