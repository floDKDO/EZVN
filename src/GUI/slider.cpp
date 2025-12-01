#include "GUI/slider.h"

#include <iostream>

Slider::Slider(const std::string path_bar, const std::string path_handle, unsigned int min_value, unsigned int max_value, const int x, const int y, std::string text, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: Ui(renderer), bar_(path_bar, x, y, renderer), handle_(path_handle, x, y - 5, renderer), min_value_(min_value), max_value_(max_value), current_value_((max_value + min_value) / 2), 
	is_dragged_(false), is_selected_(false), delta_mouse_handle_x_(0), 
	text_(text, {255, 255, 255, 255}, "fonts/Aller_Rg.ttf", 50, x, y - bar_.position_.h*3, renderer) //TODO : pas de couleur fixée... => paramètre
{
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = false;
	handle_.position_.x += (float(current_value_ - min_value_) / float(max_value_ - min_value_)) * (bar_.position_.w - handle_.position_.w);
}

//TODO : position dans Image, Text et Ui... => pk pas retirer la position de Ui vu qu'on copie celle de l'Image/Text dans celle de Ui ?

bool Slider::is_mouse_on_handle(int mouse_x, int mouse_y)
{
	float logical_x, logical_y;
	SDL_RenderWindowToLogical(renderer_.fetch(), mouse_x, mouse_y, &logical_x, &logical_y);

	return (handle_.position_.y + handle_.position_.h > logical_y
		 && handle_.position_.y < logical_y
		 && handle_.position_.x + handle_.position_.w > logical_x
		 && handle_.position_.x < logical_x);
}

void Slider::handle_movement()
{
	int logical_mouse_x, logical_mouse_y;
	get_logical_mouse_position(&logical_mouse_x, &logical_mouse_y);

	if(is_dragged_)
	{
		if(logical_mouse_x > delta_mouse_handle_x_) //TODO : code presque dupliqué dans la méthode on_pointer_down()
		{
			if(logical_mouse_x - delta_mouse_handle_x_ < bar_.position_.x - (handle_.position_.w / 2))
			{
				handle_.position_.x = bar_.position_.x - (handle_.position_.w / 2);
			}
			else if(logical_mouse_x - delta_mouse_handle_x_ > bar_.position_.x + bar_.position_.w - (handle_.position_.w / 2))
			{
				handle_.position_.x = bar_.position_.x + bar_.position_.w - (handle_.position_.w / 2);
			}
			else
			{
				handle_.position_.x = logical_mouse_x - delta_mouse_handle_x_;
			}
		}
	}
}

void Slider::unselect()
{
	is_dragged_ = false;
	is_selected_ = false;
}

void Slider::on_pointer_up()
{
	if(is_dragged_)
	{
		unselect();
		Ui::on_pointer_up();
	}
}

void Slider::on_pointer_down_hook_end()
{
	int logical_mouse_x, logical_mouse_y;
	get_logical_mouse_position(&logical_mouse_x, &logical_mouse_y);

	if(logical_mouse_x - (handle_.position_.w / 2) < bar_.position_.x)
	{
		handle_.position_.x = bar_.position_.x - (handle_.position_.w / 2);
	}
	else if(logical_mouse_x - (handle_.position_.w / 2) > bar_.position_.x + bar_.position_.w - (handle_.position_.w / 2))
	{
		handle_.position_.x = bar_.position_.x + bar_.position_.w - (handle_.position_.w / 2);
	}
	else
	{
		handle_.position_.x = logical_mouse_x - (handle_.position_.w / 2);
	}
	delta_mouse_handle_x_ = logical_mouse_x - handle_.position_.x;
	is_dragged_ = true;
}

void Slider::on_pointer_enter_hook_end()
{
	handle_movement();
}

void Slider::on_pointer_exit_hook_end()
{
	handle_movement();
}

void Slider::on_up_pressed()
{
	if(!is_selected_)
	{
		Ui::on_up_pressed();
	}
}

void Slider::on_down_pressed()
{
	if(!is_selected_)
	{
		Ui::on_down_pressed();
	}
}

void Slider::on_left_pressed()
{
	if(is_selected_)
	{
		if(lock_ && state_ == State::SELECTED)
		{
			lock_ = false;
			handle_.position_.x -= bar_.position_.w / 10;
			if(handle_.position_.x + (handle_.position_.w / 2) < bar_.position_.x)
			{
				handle_.position_.x = bar_.position_.x - (handle_.position_.w / 2);
			}
		}
	}
	else
	{
		Ui::on_left_pressed();
	}
}

void Slider::on_right_pressed()
{
	if(is_selected_)
	{
		if(lock_ && state_ == State::SELECTED)
		{
			lock_ = false;
			handle_.position_.x += bar_.position_.w / 10;
			if(handle_.position_.x + (handle_.position_.w / 2) > bar_.position_.x + bar_.position_.w)
			{
				handle_.position_.x = bar_.position_.x + bar_.position_.w - (handle_.position_.w / 2);
			}
		}
	}
	else
	{
		Ui::on_right_pressed();
	}
}

void Slider::on_enter_pressed_hook_end()
{
	is_selected_ = !is_selected_;
}

void Slider::draw(sdl::Renderer& renderer)
{
	bar_.draw(renderer);
	handle_.draw(renderer);
	text_.draw(renderer);
}

void Slider::update()
{
	text_.update();

	if(handle_.position_.x + (handle_.position_.w / 2) == bar_.position_.x)
	{
		current_value_ = min_value_;
	}
	else if(handle_.position_.x + (handle_.position_.w / 2) == bar_.position_.x + bar_.position_.w)
	{
		current_value_ = max_value_;
	}
	else
	{
		current_value_ = min_value_ + float((handle_.position_.x + (handle_.position_.w / 2) - bar_.position_.x)) / (bar_.position_.w) * (max_value_ - min_value_);
	}
}

void Slider::handle_events_hook_end(const SDL_Event& e)
{
	//Code that allow to move the handle by clicking on it even if it go beyond the bar of the slider
	int logical_mouse_x, logical_mouse_y;
	get_logical_mouse_position(&logical_mouse_x, &logical_mouse_y);
	if(e.type == SDL_MOUSEBUTTONDOWN)
	{
		if(e.button.button == SDL_BUTTON_LEFT && is_mouse_on_handle(logical_mouse_x, logical_mouse_y))
		{
			on_pointer_down();
		}
	}
}

SDL_Rect Slider::get_rect() const
{
	return bar_.position_; //handle.position n'est normalement pas utile
}