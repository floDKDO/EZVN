#include "slider.h"

Slider::Slider(const std::string path_bar, const std::string path_handle, unsigned int min_value, unsigned int max_value, const int x, const int y, SDL_Renderer* renderer, std::function<void(Ui* ui)> callback_function)
	: bar(path_bar, x, y, renderer), handle(path_handle, x, y, renderer) /*TODO : pas x y pour l'handle, dépend de la valeur d'initialisation*/, min_value(min_value), max_value(max_value), current_value(0), is_dragged(false), diff(0)
{
	this->callback_function = callback_function;
	this->position = this->bar.position;
	//this->handle_position = this->handle.position; //TODO : problème utiliser l'un ou l'autre mais pas les deux en même temps
	this->pointer_on_ui_when_pointer_up = false;
}

//TODO : position dans Image, Text et Ui... => pk pas retirer la position de Ui vu qu'on copie celle de l'Image/Text dans celle de Ui ?
bool Slider::is_mouse_on_handle(int mouse_x, int mouse_y)
{
	return (this->handle.position.y + this->handle.position.h > mouse_y
		 && this->handle.position.y < mouse_y
		 && this->handle.position.x + this->handle.position.w > mouse_x
		 && this->handle.position.x < mouse_x);
}

void Slider::on_pointer_up(bool pointer_on_ui_when_pointer_up)
{
	this->is_dragged = false; 
	Ui::on_pointer_up(pointer_on_ui_when_pointer_up);
}

void Slider::on_pointer_down()
{
	Ui::on_pointer_down();

	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	if(is_mouse_on_handle(mouse_x, mouse_y))
	{
		this->is_dragged = true;
		diff = mouse_x - handle.position.x;
	}
	else if(is_mouse_on_ui()) 
	{
		if(mouse_x - (handle.position.w / 2) < bar.position.x)
		{
			handle.position.x = bar.position.x;
		}
		else if(mouse_x - (handle.position.w / 2) > bar.position.x + bar.position.w - handle.position.w)
		{
			handle.position.x = bar.position.x + bar.position.w - handle.position.w;
		}
		else
		{
			handle.position.x = mouse_x - (handle.position.w / 2);
		}
	}
}

void Slider::on_pointer_enter()
{
	Ui::on_pointer_enter();

	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	if(is_dragged)
	{
		if(mouse_x > diff)
		{
			if(mouse_x - diff < bar.position.x)
			{
				handle.position.x = bar.position.x;
			}
			else if(mouse_x - diff > bar.position.x + bar.position.w - handle.position.w)
			{
				handle.position.x = bar.position.x + bar.position.w - handle.position.w;
			}
			else
			{
				handle.position.x = mouse_x - diff;
			}
		}
	}
}

//TODO : solution actuelle => même code que on_pointer_enter
void Slider::on_pointer_exit()
{
	Ui::on_pointer_exit();

	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	if(is_dragged)
	{
		if(mouse_x > diff)
		{
			if(mouse_x - diff < bar.position.x)
			{
				handle.position.x = bar.position.x;
			}
			else if(mouse_x - diff > bar.position.x + bar.position.w - handle.position.w)
			{
				handle.position.x = bar.position.x + bar.position.w - handle.position.w;
			}
			else
			{
				handle.position.x = mouse_x - diff;
			}
		}
	}
}

void Slider::on_left_pressed()
{
	if(this->is_dragged)
	{
		if(this->lock && this->state == State::SELECTED)
		{
			this->lock = false;
			handle.position.x -= 20;
			if(handle.position.x < bar.position.x)
				handle.position.x = bar.position.x;
		}
	}
	else
	{
		Ui::on_left_pressed();
	}
}

void Slider::on_right_pressed()
{
	if(this->is_dragged)
	{
		if(this->lock && this->state == State::SELECTED)
		{
			this->lock = false;
			handle.position.x += 20;
			if(handle.position.x + handle.position.w > bar.position.x + bar.position.w)
				handle.position.x = bar.position.x + bar.position.w - handle.position.w;
		}
	}
	else
	{
		Ui::on_right_pressed();
	}
}

void Slider::on_enter_pressed()
{
	if(this->lock && this->state == State::SELECTED)
	{
		this->on_pointer_down();
		this->lock = false;
		this->is_dragged = !this->is_dragged;
	}
}

void Slider::draw(SDL_Renderer* renderer)
{
	this->bar.draw(renderer);
	this->handle.draw(renderer);
}

void Slider::update(Uint64& time_step)
{
	(void)time_step;
}
