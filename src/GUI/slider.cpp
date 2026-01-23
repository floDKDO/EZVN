#include "GUI/slider.h"
#include "constants.h"

#include <iostream>

const unsigned int Slider::index_rect_container_ = 0;

Slider::Slider(unsigned int min_value, unsigned int max_value, int x, int y, std::string_view text, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: Slider(min_value, max_value, (max_value + min_value) / 2, x, y, text, renderer, callback_function)
{}

Slider::Slider(unsigned int min_value, unsigned int max_value, unsigned int current_value, int x, int y, std::string_view text, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: Ui(renderer), container_({x, y, constants::slider_container_width_, constants::slider_container_height_}), container_outline_({x, y, constants::slider_container_width_, constants::slider_container_height_}),
	handle_({x, y + constants::slider_handle_y_delta_, constants::slider_handle_size_, constants::slider_handle_size_}),
	handle_outline_({x, y + constants::slider_handle_y_delta_, constants::slider_handle_size_, constants::slider_handle_size_}),
	text_(text, constants::slider_text_color_, constants::slider_font_, constants::slider_text_size_, x, y + constants::slider_text_y_delta_, renderer),
	min_value_(min_value), max_value_(max_value), current_value_(current_value), delta_mouse_handle_x_(0)
{
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = false;
	handle_.x += int((float(current_value_ - min_value_) / float(max_value_ - min_value_)) * (container_.w - handle_.w));
	handle_outline_.x = handle_.x;
	text_.position_.x += (container_.w - text_.position_.w) / 2;
}

bool Slider::is_mouse_on_handle(int mouse_x, int mouse_y) const
{
	float logical_x, logical_y;
	renderer_.window_to_logical(mouse_x, mouse_y, &logical_x, &logical_y);

	return (handle_.y + handle_.h > logical_y
		 && handle_.y < logical_y
		 && handle_.x + handle_.w > logical_x
		 && handle_.x < logical_x);
}

void Slider::disable_keyboard_focus()
{
	has_keyboard_focus_ = false;
}

void Slider::handle_movement(int mouse_x)
{
	int logical_mouse_x = mouse_x;

	if(logical_mouse_x - (handle_.w / 2) < container_.x - (handle_.w / 2))
	{
		handle_.x = container_.x - (handle_.w / 2);
		handle_outline_.x = handle_.x;
	}
	else if(logical_mouse_x - (handle_.w / 2) > container_.x + container_.w - (handle_.w / 2))
	{
		handle_.x = container_.x + container_.w - (handle_.w / 2);
		handle_outline_.x = handle_.x;
	}
	else
	{
		handle_.x = logical_mouse_x - (handle_.w / 2);
		handle_outline_.x = handle_.x;
	}
	delta_mouse_handle_x_ = logical_mouse_x - handle_.x;
}

void Slider::on_pointer_down_hook_end(PointerEventData pointer_event_data)
{
	handle_movement(pointer_event_data.mouse_x);
}

void Slider::on_drag(PointerEventData pointer_event_data)
{
	handle_movement(pointer_event_data.mouse_x);
}

void Slider::on_up_pressed()
{
	if(!has_keyboard_focus_)
	{
		Ui::on_up_pressed();
	}
}

void Slider::on_down_pressed()
{
	if(!has_keyboard_focus_)
	{
		Ui::on_down_pressed();
	}
}

void Slider::on_left_pressed()
{
	if(has_keyboard_focus_)
	{
		handle_.x -= container_.w / constants::slider_step_count_;
		handle_outline_.x = handle_.x;
		if(handle_.x + (handle_.w / 2) < container_.x)
		{
			handle_.x = container_.x - (handle_.w / 2);
			handle_outline_.x = handle_.x;
		}
	}
	else
	{
		Ui::on_left_pressed();
	}
}

void Slider::on_right_pressed()
{
	if(has_keyboard_focus_)
	{
		handle_.x += container_.w / constants::slider_step_count_;
		handle_outline_.x = handle_.x;
		if(handle_.x + (handle_.w / 2) > container_.x + container_.w)
		{
			handle_.x = container_.x + container_.w - (handle_.w / 2);
			handle_outline_.x = handle_.x;
		}
	}
	else
	{
		Ui::on_right_pressed();
	}
}

void Slider::on_enter_pressed_hook_end()
{
	has_keyboard_focus_ = !has_keyboard_focus_;
}

void Slider::draw(sdl::Renderer& renderer)
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

	text_.draw(renderer);
}

void Slider::update()
{
	text_.update();

	if(handle_.x + (handle_.w / 2) == container_.x)
	{
		current_value_ = min_value_;
	}
	else if(handle_.x + (handle_.w / 2) == container_.x + container_.w)
	{
		current_value_ = max_value_;
	}
	else
	{
		current_value_ = min_value_ + unsigned int (float((handle_.x + (handle_.w / 2) - container_.x)) / (container_.w) * (max_value_ - min_value_));
	}
}

//TODO : pas ouf et ne fonctionne plus
/*void Slider::handle_events_hook_end(const SDL_Event& e)
{
	//Code that allow to move the handle by pressing on it even if it go beyond the container of the slider
	int logical_mouse_x, logical_mouse_y;
	get_logical_mouse_position(&logical_mouse_x, &logical_mouse_y);
	if(e.type == SDL_MOUSEBUTTONDOWN)
	{
		if(e.button.button == SDL_BUTTON_LEFT && is_mouse_on_handle(logical_mouse_x, logical_mouse_y))
		{
			on_drag();
		}
	}
}*/

SDL_Rect Slider::get_rect() const
{
	return container_; //handle_ n'est normalement pas utile
}