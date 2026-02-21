#include "GUI/scrollbar.h"
#include "constants.h"
#include "utils.h"

#include <algorithm>
#include <iostream>

Scrollbar::Scrollbar(int x, int y, int container_h, int handle_h, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: UiWidget(renderer), current_value_(0.0f), min_value_(0.0f), max_value_(1.0f),
	container_({x, y, constants::scrollbar_container_width_, container_h}),
	container_outline_({x, y, constants::scrollbar_container_width_, container_h}),
	handle_({x, y, constants::scrollbar_handle_width_, handle_h}), 
	handle_outline_({x, y, constants::scrollbar_handle_width_, handle_h}), 
	up_triangle_("img/gui/triangle_scrollbar.png", "img/gui/triangle_scrollbar.png", "img/gui/triangle_scrollbar.png", "", x + 4, y - 10, renderer, callback_function), //TODO : hardcodé
	down_triangle_("img/gui/triangle_scrollbar_reversed.png", "img/gui/triangle_scrollbar_reversed.png", "img/gui/triangle_scrollbar_reversed.png", "", x + 4, y + container_h + 2, renderer, callback_function), //TODO : hardcodé
	delta_mouse_handle_y_(0), step_y_(container_h / constants::scrollbar_step_count_), info_when_click_({0, 0, InfoWhenClick::WhereWasMouse::ABOVE, false}),
	mouse_is_on_handle_(false)
{
	handle_.y = container_.y;
	handle_outline_.y = container_.y;
	handle_.x += (constants::scrollbar_container_width_ - constants::scrollbar_handle_width_) / 2; 
	handle_outline_.x += (constants::scrollbar_container_width_ - constants::scrollbar_handle_width_) / 2; 
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = false;
	callback_called_when_pointer_up_ = false; //= down

	rect_ = container_;
}

int Scrollbar::get_low_bound() const
{
	return container_.y;
}

int Scrollbar::get_high_bound() const
{
	return container_.y + container_.h - handle_.h;
}

void Scrollbar::update_current_value()
{
	int low = get_low_bound();
	int high = get_high_bound();

	if(handle_.y == low)
	{
		current_value_ = min_value_;
	}
	else if(handle_.y == high)
	{
		current_value_ = max_value_;
	}
	else
	{
		current_value_ = min_value_ + float((handle_.y - container_.y)) / (container_.h - handle_.h) * (max_value_ - min_value_);
	}
}

void Scrollbar::disable_keyboard_focus()
{
	has_keyboard_focus_ = false;
}

void Scrollbar::handle_movement(int mouse_x, int mouse_y)
{
	int low = get_low_bound();
	int high = get_high_bound();

	if(handle_.y < low)
	{
		handle_.y = low;
	}
	else if(handle_.y > high)
	{
		handle_.y = high;
	}
	else
	{
		if(info_when_click_.where_was_mouse_ == InfoWhenClick::WhereWasMouse::ON)
		{
			int new_handle_y_pos = std::clamp(info_when_click_.handle_y_pos_ + (mouse_y - info_when_click_.mouse_y_pos_), low, high);
			handle_.y = new_handle_y_pos;
		}
		else
		{
			if(utils::is_point_in_rect({mouse_x, mouse_y}, handle_))
			{
				mouse_is_on_handle_ = true;
			}
			else
			{
				if(!mouse_is_on_handle_)
				{
					if(info_when_click_.where_was_mouse_ == InfoWhenClick::WhereWasMouse::ABOVE)
					{
						handle_.y = std::clamp(handle_.y - step_y_, low, high);
					}
					else if(info_when_click_.where_was_mouse_ == InfoWhenClick::WhereWasMouse::BELOW)
					{
						handle_.y = std::clamp(handle_.y + step_y_, low, high);
					}
				}
				else
				{
					if(info_when_click_.where_was_mouse_ == InfoWhenClick::WhereWasMouse::ABOVE && mouse_y <= handle_.y)
					{
						handle_.y = std::clamp(handle_.y - handle_.h, low, high);
					}
					else if(info_when_click_.where_was_mouse_ == InfoWhenClick::WhereWasMouse::BELOW && mouse_y >= handle_.y + handle_.h)
					{
						handle_.y = std::clamp(handle_.y + handle_.h, low, high);
					}
				}
			}
		}
	}
	handle_outline_.y = handle_.y;
	delta_mouse_handle_y_ = mouse_y - handle_.y;
}

std::vector<UiWidget*> Scrollbar::get_navigation_nodes()
{
	std::vector<UiWidget*> widgets;
	widgets.push_back(this); //TODO : pas ouf => this contient déjà up_triangle_ et down_triangle_
	widgets.push_back(&up_triangle_);
	widgets.push_back(&down_triangle_);
	return widgets;
}

void Scrollbar::on_pointer_down_hook_end(PointerEventData pointer_event_data)
{
	if(!info_when_click_.saved_)
	{
		InfoWhenClick::WhereWasMouse where_was_mouse;

		if(utils::is_point_in_rect({pointer_event_data.mouse_x_, pointer_event_data.mouse_y_}, handle_))
		{
			where_was_mouse = InfoWhenClick::WhereWasMouse::ON;
		}
		else
		{
			if(pointer_event_data.mouse_y_ < handle_.y) //la souris était au-dessus
			{
				where_was_mouse = InfoWhenClick::WhereWasMouse::ABOVE;
			}
			else
			{
				where_was_mouse = InfoWhenClick::WhereWasMouse::BELOW;
			}
		}
		info_when_click_ = {pointer_event_data.mouse_y_, handle_.y, where_was_mouse, true};
	}
	handle_movement(pointer_event_data.mouse_x_, pointer_event_data.mouse_y_);

	update_current_value();
	callback_function_(this);
}

void Scrollbar::on_pointer_up_hook_end([[maybe_unused]] PointerEventData pointer_event_data)
{
	info_when_click_ = {0, 0, InfoWhenClick::WhereWasMouse::ABOVE, false};
}

void Scrollbar::on_drag(PointerEventData pointer_event_data)
{
	handle_movement(pointer_event_data.mouse_x_, pointer_event_data.mouse_y_);

	update_current_value();
	callback_function_(this);
}

void Scrollbar::on_mouse_wheel(Sint32 mouse_wheel_y)
{
	int low = get_low_bound();
	int high = get_high_bound();

	if(mouse_wheel_y > 0) //scroll vers l'avant
	{
		handle_.y = std::clamp(handle_.y - (step_y_ / 2), low, high); //(step_y_ / 2) pour ralentir un peu la vitesse du scroll
	}
	else //scroll vers l'arrière
	{
		handle_.y = std::clamp(handle_.y + (step_y_ / 2), low, high); //(step_y_ / 2) pour ralentir un peu la vitesse du scroll
	}
	handle_outline_.y = handle_.y;

	update_current_value();
	callback_function_(this);
}

void Scrollbar::on_up_pressed()
{
	if(has_keyboard_focus_)
	{
		handle_.y -= container_.h / constants::scrollbar_step_count_;
		handle_outline_.y = handle_.y;

		int low = get_low_bound();
		if(handle_.y < low)
		{
			handle_.y = low; 
			handle_outline_.y = handle_.y;
		}

		update_current_value();
		callback_function_(this); 
	}
	else
	{
		UiWidget::on_up_pressed();
	}
}

void Scrollbar::on_down_pressed()
{
	if(has_keyboard_focus_)
	{
		handle_.y += container_.h / constants::scrollbar_step_count_;
		handle_outline_.y = handle_.y;

		int high = get_high_bound();
		if(handle_.y > high)
		{
			handle_.y = high; 
			handle_outline_.y = handle_.y;
		}

		update_current_value();
		callback_function_(this); 
	}
	else
	{
		UiWidget::on_down_pressed();
	}
}

void Scrollbar::on_left_pressed()
{
	if(!has_keyboard_focus_)
	{
		UiWidget::on_left_pressed();
	}
}

void Scrollbar::on_right_pressed()
{
	if(!has_keyboard_focus_)
	{
		UiWidget::on_right_pressed();
	}
}

void Scrollbar::on_enter_pressed_hook_end()
{
	has_keyboard_focus_ = !has_keyboard_focus_;
}

void Scrollbar::draw(sdl::Renderer& renderer)
{
	renderer.set_draw_color(constants::scrollbar_container_color_.r_, constants::scrollbar_container_color_.g_, constants::scrollbar_container_color_.b_, constants::scrollbar_container_color_.a_);
	renderer.fill_rect(&container_);

	renderer.set_draw_color(constants::scrollbar_container_outline_color_.r_, constants::scrollbar_container_outline_color_.g_, constants::scrollbar_container_outline_color_.b_, constants::scrollbar_container_outline_color_.a_);
	renderer.draw_rect(&container_outline_);

	if(state_ == State::NORMAL)
	{
		renderer.set_draw_color(constants::scrollbar_handle_normal_color_.r_, constants::scrollbar_handle_normal_color_.g_, constants::scrollbar_handle_normal_color_.b_, constants::scrollbar_handle_normal_color_.a_);
	}
	else if(state_ == State::SELECTED)
	{
		renderer.set_draw_color(constants::scrollbar_handle_selected_color_.r_, constants::scrollbar_handle_selected_color_.g_, constants::scrollbar_handle_selected_color_.b_, constants::scrollbar_handle_selected_color_.a_);
	}
	else if(state_ == State::PRESSED)
	{
		renderer.set_draw_color(constants::scrollbar_handle_pressed_color_.r_, constants::scrollbar_handle_pressed_color_.g_, constants::scrollbar_handle_pressed_color_.b_, constants::scrollbar_handle_pressed_color_.a_);
	}
	renderer.fill_rect(&handle_);

	renderer.set_draw_color(constants::scrollbar_handle_outline_color_.r_, constants::scrollbar_handle_outline_color_.g_, constants::scrollbar_handle_outline_color_.b_, constants::scrollbar_handle_outline_color_.a_);
	renderer.draw_rect(&handle_outline_);

	up_triangle_.draw(renderer);
	down_triangle_.draw(renderer);
}

void Scrollbar::update()
{
	up_triangle_.update();
	down_triangle_.update();
}

void Scrollbar::change_position(int x, int y)
{
	container_.x = x;
	container_.y = y;

	container_outline_.x = x;
	container_outline_.y = y;

	handle_.x = x + (constants::scrollbar_container_width_ - constants::scrollbar_handle_width_) / 2;
	handle_.y = container_.y;

	handle_outline_.x = x + (constants::scrollbar_container_width_ - constants::scrollbar_handle_width_) / 2;
	handle_outline_.y = container_.y;

	up_triangle_.change_position(x, y);
	down_triangle_.change_position(x, y);

	rect_ = container_;
}