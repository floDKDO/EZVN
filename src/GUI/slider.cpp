#include "GUI/slider.h"

#include <iostream>

Slider::Slider(const std::string path_bar, const std::string path_handle, unsigned int min_value, unsigned int max_value, const int x, const int y, std::string text, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: Ui(renderer), bar(path_bar, x, y, renderer), handle(path_handle, x, y - 5, renderer), min_value(min_value), max_value(max_value), current_value(max_value/2), is_dragged(false), is_selected(false), diff(0),
	text(text, {255, 255, 255, 255}, "fonts/Aller_Rg.ttf", 50, x, y - bar.position.h*3, renderer) //TODO : pas de couleur fixée...
{
	this->callback_function = callback_function;
	this->pointer_on_ui_when_pointer_up = false;
	//this->renderer = renderer;

	this->handle.position.x += (float(current_value - min_value) / float(max_value - min_value)) * (this->bar.position.w - this->handle.position.w);
}

//TODO : position dans Image, Text et Ui... => pk pas retirer la position de Ui vu qu'on copie celle de l'Image/Text dans celle de Ui ?

bool Slider::is_mouse_on_handle(int mouse_x, int mouse_y)
{
	float logical_x, logical_y;
	SDL_RenderWindowToLogical(this->renderer.Get(), mouse_x, mouse_y, &logical_x, &logical_y);

	return (this->handle.position.y + this->handle.position.h > logical_y
		 && this->handle.position.y < logical_y
		 && this->handle.position.x + this->handle.position.w > logical_x
		 && this->handle.position.x < logical_x);
}

void Slider::handle_movement()
{
	int logical_mouse_x, logical_mouse_y;
	this->get_logical_mouse_position(&logical_mouse_x, &logical_mouse_y);

	if(is_dragged)
	{
		if(logical_mouse_x > diff) //TODO : code presque dupliqué dans la méthode on_pointer_down()
		{
			if(logical_mouse_x - diff < bar.position.x - (handle.position.w / 2))
			{
				handle.position.x = bar.position.x - (handle.position.w / 2);
			}
			else if(logical_mouse_x - diff > bar.position.x + bar.position.w - (handle.position.w / 2))
			{
				handle.position.x = bar.position.x + bar.position.w - (handle.position.w / 2);
			}
			else
			{
				handle.position.x = logical_mouse_x - diff;
			}
		}
	}
}

void Slider::on_pointer_up()
{
	if(this->is_dragged)
	{
		this->is_dragged = false;
		this->is_selected = false;
		Ui::on_pointer_up();
	}
}

void Slider::on_pointer_down_hook_end()
{
	int logical_mouse_x, logical_mouse_y;
	this->get_logical_mouse_position(&logical_mouse_x, &logical_mouse_y);

	if(logical_mouse_x - (handle.position.w / 2) < bar.position.x)
	{
		handle.position.x = bar.position.x - (handle.position.w / 2);
	}
	else if(logical_mouse_x - (handle.position.w / 2) > bar.position.x + bar.position.w - (handle.position.w / 2))
	{
		handle.position.x = bar.position.x + bar.position.w - (handle.position.w / 2);
	}
	else
	{
		handle.position.x = logical_mouse_x - (handle.position.w / 2);
	}
	diff = logical_mouse_x - handle.position.x;
	this->is_dragged = true;
}

void Slider::on_pointer_enter_hook_end()
{
	this->handle_movement();
}

void Slider::on_pointer_exit_hook_end()
{
	this->handle_movement();
}

void Slider::on_up_pressed()
{
	if(!this->is_selected)
	{
		Ui::on_up_pressed();
	}
}

void Slider::on_down_pressed()
{
	if(!this->is_selected)
	{
		Ui::on_down_pressed();
	}
}

void Slider::on_left_pressed()
{
	if(this->is_selected)
	{
		if(this->lock && this->state == State::SELECTED)
		{
			this->lock = false;
			handle.position.x -= this->bar.position.w / 10;
			if(handle.position.x + (handle.position.w / 2) < bar.position.x)
			{
				handle.position.x = bar.position.x - (handle.position.w / 2);
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
	if(this->is_selected)
	{
		if(this->lock && this->state == State::SELECTED)
		{
			this->lock = false;
			handle.position.x += this->bar.position.w / 10;
			if(handle.position.x + (handle.position.w / 2) > bar.position.x + bar.position.w)
			{
				handle.position.x = bar.position.x + bar.position.w - (handle.position.w / 2);
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
	this->is_selected = !this->is_selected;
}

void Slider::draw(sdl::Renderer& renderer)
{
	this->bar.draw(renderer);
	this->handle.draw(renderer);
	this->text.draw(renderer);
}

void Slider::update(Uint64 time_step)
{
	(void)time_step;

	this->text.update(time_step);

	if(this->handle.position.x + (handle.position.w / 2) == this->bar.position.x)
	{
		this->current_value = this->min_value;
	}
	else if(this->handle.position.x + (handle.position.w / 2) == this->bar.position.x + this->bar.position.w)
	{
		this->current_value = this->max_value;
	}
	else
	{
		this->current_value = this->min_value + float((this->handle.position.x + (this->handle.position.w / 2) - this->bar.position.x)) / (this->bar.position.w) * 100;
	}
}

void Slider::handle_events_hook_end(const SDL_Event& e)
{
	//Code that allow to move the handle by clicking on it even if it go beyond the bar of the slider
	int logical_mouse_x, logical_mouse_y;
	this->get_logical_mouse_position(&logical_mouse_x, &logical_mouse_y);
	if(e.type == SDL_MOUSEBUTTONDOWN)
	{
		if(e.button.button == SDL_BUTTON_LEFT && this->is_mouse_on_handle(logical_mouse_x, logical_mouse_y))
		{
			this->on_pointer_down();
		}
	}
}

SDL_Rect Slider::get_rect() const
{
	return this->bar.position; //this->handle.position n'est normalement pas utile
}