#include "GUI/scrollbar.h"
#include "constants.h"

#include <algorithm>
#include <iostream>

Scrollbar::Scrollbar(int x, int y, int h, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: UiWidget(renderer), current_value_(0.0f), min_value_(0.0f), max_value_(1.0f),
	container_({x, y, constants::slider_container_height_, h}), 
	container_outline_({x, y, constants::slider_container_height_, h}),
	handle_({x, y + constants::slider_handle_y_delta_, constants::slider_container_height_ - 2, 100}), //TODO : hardcodé
	handle_outline_({x, y + constants::slider_handle_y_delta_, constants::slider_container_height_ - 2, 100}), //TODO : hardcodé
	delta_mouse_handle_y_(0), info_when_click_({0, 0, false, WhereWasMouse::ABOVE, false, false})
{
	handle_.y = container_.y;
	handle_outline_.y = container_.y;
	handle_.x += 1;
	handle_outline_.x += 1;
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = false;
	callback_called_when_pointer_up_ = false; //= down

	rect_ = container_;
}

//TODO : pas possible d'agir sur la scrollbar avec le clavier

//TODO : faire grandir la barre selon ce qu'il y a dans la scrollable area ??

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

void Scrollbar::handle_movement(int mouse_x, int mouse_y)
{
	int logical_mouse_y = mouse_y; //TODO : variable inutile
	int low = container_.y;
	int high = container_.y + container_.h - handle_.h;

	if(handle_.y < container_.y)
	{
		handle_.y = low;
		handle_outline_.y = handle_.y;
	}
	else if(handle_.y + handle_.h > container_.y + container_.h)
	{
		handle_.y = high;
		handle_outline_.y = handle_.y;
	}
	else
	{
		if(info_when_click_.mouse_was_on_handle_) 
		{
			//std::cout << "CAS COLLISION\n";
			int new_handle_y_pos = std::clamp(info_when_click_.handle_y_pos_ + (logical_mouse_y - info_when_click_.mouse_y_pos_), low, high);
			handle_.y = new_handle_y_pos;
			handle_outline_.y = handle_.y;
		}
		else
		{
			//std::cout << "CAS PAS DE COLLISION\n";

			if(info_when_click_.where_was_mouse_ == WhereWasMouse::ABOVE)
			{
				SDL_Point p = {mouse_x, mouse_y};
				if(SDL_PointInRect(&p, &handle_)) 
				{
					info_when_click_.mouse_is_on_handle_ = true;
				}
				else 
				{
					if(!info_when_click_.mouse_is_on_handle_)
					{
						handle_.y -= 20; //TODO : hardcodé
					}
					else
					{
						if(logical_mouse_y <= handle_.y)
						{
							handle_.y = std::clamp(handle_.y - handle_.h, low, high);
							//std::cout << "***********\n";
						}
					}
					handle_outline_.y = handle_.y;
				}
			}
			else //BELOW
			{
				SDL_Point p = {mouse_x, mouse_y};
				if(SDL_PointInRect(&p, &handle_)) 
				{
					info_when_click_.mouse_is_on_handle_ = true; 
				}
				else 
				{
					if(!info_when_click_.mouse_is_on_handle_)
					{
						handle_.y += 20; //TODO : hardcodé
					}
					else
					{
						std::cout << logical_mouse_y << ", " << handle_.y + handle_.h << std::endl;
						if(logical_mouse_y >= handle_.y + handle_.h)
						{
							handle_.y = std::clamp(handle_.y + handle_.h, low, high);
							//std::cout << "############\n";
						}
					}
					handle_outline_.y = handle_.y;
				}
			}
		}
	}
	delta_mouse_handle_y_ = logical_mouse_y - handle_.y;
}

void Scrollbar::on_pointer_down_hook_end(PointerEventData pointer_event_data)
{
	if(!info_when_click_.saved_)
	{
		SDL_Point p = {pointer_event_data.mouse_x_, pointer_event_data.mouse_y_};
		bool mouse_on_handle = false;
		WhereWasMouse where_was_mouse;
		if(SDL_PointInRect(&p, &handle_)) 
		{
			mouse_on_handle = true;
			where_was_mouse = WhereWasMouse::ON;
		}
		else
		{
			if(pointer_event_data.mouse_y_ < handle_.y) //la souris était au-dessus
			{
				where_was_mouse = WhereWasMouse::ABOVE;
			}
			else
			{
				where_was_mouse = WhereWasMouse::BELOW;
			}
		}
		info_when_click_ = {pointer_event_data.mouse_y_, handle_.y, mouse_on_handle, where_was_mouse, false, true};
	}
	handle_movement(pointer_event_data.mouse_x_, pointer_event_data.mouse_y_);
}

void Scrollbar::on_pointer_up_hook_end(PointerEventData pointer_event_data)
{
	info_when_click_ = {0, 0, false, WhereWasMouse::ABOVE, false, false};
}

void Scrollbar::on_drag(PointerEventData pointer_event_data)
{
	callback_function_(this);
	handle_movement(pointer_event_data.mouse_x_, pointer_event_data.mouse_y_);
}

void Scrollbar::on_up_pressed()
{
	if(has_keyboard_focus_)
	{
		//callback_function_(this); //TODO : ok (pour que les éléments de la zone à scroller se déplacent en temps réel) mais deux appels à callback_function_ (=> dans on_enter_released() aussi)
		handle_.y -= container_.h / constants::slider_step_count_;
		handle_outline_.y = handle_.y;
		if(handle_.y < container_.y)
		{
			handle_.y = container_.y; //TODO : pareil que la variable low dans handle_movement()...
			handle_outline_.y = handle_.y;
		}
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
		//callback_function_(this); //TODO : ok (pour que les éléments de la zone à scroller se déplacent en temps réel) mais deux appels à callback_function_ (=> dans on_enter_released() aussi)
		handle_.y += container_.h / constants::slider_step_count_;
		handle_outline_.y = handle_.y;
		if(handle_.y + handle_.h > container_.y + container_.h)
		{
			handle_.y = container_.y + container_.h - handle_.h; //TODO : pareil que la variable high dans handle_movement()...
			handle_outline_.y = handle_.y;
		}
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
	if(handle_.y == container_.y)
	{
		current_value_ = min_value_;
	}
	else if(handle_.y == container_.y + container_.h)
	{
		current_value_ = max_value_;
	}
	else
	{
		current_value_ = min_value_ + float((handle_.y - container_.y)) / (container_.h - handle_.h) * (max_value_ - min_value_);
	}
}

void Scrollbar::change_position(int x, int y)
{
	container_.x = x;
	container_.y = y;

	container_outline_.x = x;
	container_outline_.y = y;

	handle_.x = x;
	handle_.y = y + constants::slider_handle_y_delta_;

	handle_outline_.x = x;
	handle_outline_.y = y + constants::slider_handle_y_delta_;

	rect_ = container_;
}