#include "GUI/scrollbar.h"
#include "constants.h"

#include <iostream>

Scrollbar::Scrollbar(int x, int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: Ui(renderer), current_value_(0.0f), min_value_(0.0f), max_value_(1.0f),
	container_({x, y, constants::slider_container_height_, constants::slider_container_width_}), 
	container_outline_({x, y, constants::slider_container_height_, constants::slider_container_width_}),
	handle_({x, y + constants::slider_handle_y_delta_, constants::slider_handle_size_, constants::slider_handle_size_}),
	handle_outline_({x, y + constants::slider_handle_y_delta_, constants::slider_handle_size_, constants::slider_handle_size_}),
	delta_mouse_handle_y_(0)
{
	handle_.y = container_.y - handle_.w / 2;
	handle_outline_.y = container_.y - handle_outline_.w / 2;
	handle_.x -= handle_.w / 4;
	handle_outline_.x -= handle_outline_.w / 4;
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = false;
	callback_called_when_pointer_up_ = false; //= down
}

//TODO : pas possible d'agir sur la scrollbar avec le clavier

//TODO : faire garndir la barre selon ce qu'il y a dans la scrollable area ??

bool Scrollbar::is_mouse_on_handle(int mouse_x, int mouse_y) const
{
	float logical_x, logical_y;
	renderer_.window_to_logical(mouse_x, mouse_y, &logical_x, &logical_y);

	return (handle_.y + handle_.h > logical_y
		 && handle_.y < logical_y
		 && handle_.x + handle_.w > logical_x
		 && handle_.x < logical_x);
}

void Scrollbar::disable_keyboard_focus()
{
	has_keyboard_focus_ = false;
}

void Scrollbar::handle_movement(int mouse_y)
{
	int logical_mouse_y = mouse_y;

	if(logical_mouse_y - (handle_.h / 2) < container_.y - (handle_.h / 2))
	{
		handle_.y = container_.y - (handle_.h / 2);
		handle_outline_.y = handle_.y;
	}
	else if(logical_mouse_y - (handle_.h / 2) > container_.y + container_.h - (handle_.h / 2))
	{
		handle_.y = container_.y + container_.h - (handle_.h / 2);
		handle_outline_.y = handle_.y;
	}
	else
	{
		handle_.y = logical_mouse_y - (handle_.h / 2);
		handle_outline_.y = handle_.y;
	}
	delta_mouse_handle_y_ = logical_mouse_y - handle_.y;
}

void Scrollbar::on_pointer_down_hook_end(PointerEventData pointer_event_data)
{
	handle_movement(pointer_event_data.mouse_y_);
}

void Scrollbar::on_drag(PointerEventData pointer_event_data)
{
	callback_function_(this);
	handle_movement(pointer_event_data.mouse_y_);
}

void Scrollbar::on_up_pressed()
{
	if(has_keyboard_focus_)
	{
		handle_.y -= container_.h / constants::slider_step_count_;
		handle_outline_.y = handle_.y;
		if(handle_.y + (handle_.h / 2) < container_.y)
		{
			handle_.y = container_.y - (handle_.h / 2);
			handle_outline_.y = handle_.y;
		}
	}
	else
	{
		Ui::on_up_pressed();
	}
}

void Scrollbar::on_down_pressed()
{
	if(has_keyboard_focus_)
	{
		handle_.y += container_.h / constants::slider_step_count_;
		handle_outline_.y = handle_.y;
		if(handle_.y + (handle_.h / 2) > container_.y + container_.h)
		{
			handle_.y = container_.y + container_.h - (handle_.h / 2);
			handle_outline_.y = handle_.y;
		}
	}
	else
	{
		Ui::on_down_pressed();
	}
}

void Scrollbar::on_left_pressed()
{
	if(!has_keyboard_focus_)
	{
		Ui::on_left_pressed();
	}
}

void Scrollbar::on_right_pressed()
{
	if(!has_keyboard_focus_)
	{
		Ui::on_right_pressed();
	}
}

void Scrollbar::on_enter_pressed_hook_end()
{
	has_keyboard_focus_ = !has_keyboard_focus_;
}

void Scrollbar::draw(sdl::Renderer& renderer)
{
	renderer.set_draw_color(constants::slider_container_color_.r_, constants::slider_container_color_.g_, constants::slider_container_color_.b_, constants::slider_container_color_.a_);
	renderer.fill_rect(&container_);

	renderer.set_draw_color(constants::slider_container_outline_color_.r_, constants::slider_container_outline_color_.g_, constants::slider_container_outline_color_.b_, constants::slider_container_outline_color_.a_);
	renderer.draw_rect(&container_outline_);

	if(state_ == State::NORMAL)
	{
		renderer.set_draw_color(constants::slider_handle_normal_color_.r_, constants::slider_handle_normal_color_.g_, constants::slider_handle_normal_color_.b_, constants::slider_handle_normal_color_.a_);
	}
	else if(state_ == State::SELECTED)
	{
		renderer.set_draw_color(constants::slider_handle_selected_color_.r_, constants::slider_handle_selected_color_.g_, constants::slider_handle_selected_color_.b_, constants::slider_handle_selected_color_.a_);
	}
	else if(state_ == State::PRESSED)
	{
		renderer.set_draw_color(constants::slider_handle_pressed_color_.r_, constants::slider_handle_pressed_color_.g_, constants::slider_handle_pressed_color_.b_, constants::slider_handle_pressed_color_.a_);
	}
	renderer.fill_rect(&handle_);

	renderer.set_draw_color(constants::slider_handle_outline_color_.r_, constants::slider_handle_outline_color_.g_, constants::slider_handle_outline_color_.b_, constants::slider_handle_outline_color_.a_);
	renderer.draw_rect(&handle_outline_);
}

void Scrollbar::update()
{
	if(handle_.y + (handle_.h / 2) == container_.y)
	{
		current_value_ = min_value_;
	}
	else if(handle_.y + (handle_.h / 2) == container_.y + container_.h)
	{
		current_value_ = max_value_;
	}
	else
	{
		current_value_ = min_value_ + float((handle_.y + (handle_.h / 2) - container_.y)) / (container_.h) * (max_value_ - min_value_);
	}
}

SDL_Rect Scrollbar::get_rect() const
{
	return container_; //handle_ n'est normalement pas utile
}