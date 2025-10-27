#include "slider.h"
#include <iostream>

Slider::Slider(const std::string path_bar, const std::string path_handle, unsigned int min_value, unsigned int max_value, const int x, const int y, SDL_Renderer* renderer, std::function<void(Ui* ui)> callback_function)
	: bar(path_bar, x, y, renderer), handle(path_handle, x, y, renderer) /*TODO : pas x y pour l'handle, dépend de la valeur d'initialisation*/, min_value(min_value), max_value(max_value), current_value(0), is_dragged(false), diff(0)
{
	this->callback_function = callback_function;
	this->position = this->bar.position;
	//this->handle_position = this->handle.position; //TODO : problème utiliser l'un ou l'autre mais pas les deux en même temps
}

//TODO : position dans Image, Text et Ui... => pk pas retirer la position de Ui vu qu'on copie celle de l'Image/Text dans celle de Ui ?
bool Slider::is_mouse_on_handle(int mouse_x, int mouse_y)
{
	return (this->handle.position.y + this->handle.position.h > mouse_y
		 && this->handle.position.y < mouse_y
		 && this->handle.position.x + this->handle.position.w > mouse_x
		 && this->handle.position.x < mouse_x);
}

void Slider::on_pointer_up()
{
	this->is_dragged = false;
	Ui::on_pointer_up();
}

void Slider::on_pointer_down()
{
	Ui::on_pointer_down();
	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	//std::cout << "**************************************************mouse button down! " << "mouse_x : " << mouse_x << " et " << handle.position.x << std::endl;

	if(is_mouse_on_handle(mouse_x, mouse_y))
	{
		this->is_dragged = true;
		diff = mouse_x - handle.position.x;
	}
	else if(is_mouse_on_ui()) //TODO : pourquoi "/ 2" ?
	{
		if(mouse_x - handle.position.w / 2 < bar.position.x)
		{
			handle.position.x = bar.position.x;
		}
		else if(mouse_x - handle.position.w / 2 > bar.position.x + bar.position.w - handle.position.w)
		{
			handle.position.x = bar.position.x + bar.position.w - handle.position.w;
		}
		else
		{
			handle.position.x = mouse_x - handle.position.w / 2;
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

void Slider::on_input_pressed(const SDL_Event& e)
{
	if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A || e.key.keysym.sym == SDLK_RETURN)
	{
		if(/*this->lock &&*/ this->state == State::SELECTED)
		{
			this->is_dragged = !this->is_dragged;
			//this->lock = false;
		}
	}

	if(this->is_dragged)
	{
		if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT || e.key.keysym.sym == SDLK_LEFT)
		{
			if(this->lock && this->state == State::SELECTED)
			{
				this->lock = false;
				handle.position.x -= 20;
				if(handle.position.x < bar.position.x)
					handle.position.x = bar.position.x;
			}
		}
		else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT || e.key.keysym.sym == SDLK_RIGHT)
		{
			if(this->lock && this->state == State::SELECTED)
			{
				this->lock = false;
				handle.position.x += 20;
				if(handle.position.x + handle.position.w > bar.position.x + bar.position.w)
					handle.position.x = bar.position.x + bar.position.w - handle.position.w;
			}
		}
	}
	else
	{
		Ui::on_input_pressed(e);
	}
}


void Slider::on_input_released(const SDL_Event& e)
{
	if(this->is_dragged)
	{
		if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A || e.key.keysym.sym == SDLK_RETURN)
		{
			if(this->state == State::CLICKED)
			{
				this->is_dragged = false;
				this->lock = true;
				this->is_selected_sound_played = false;
			}
		}
	}
	else
	{
		Ui::on_input_released(e);
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
	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	//std::cout << "is_dragged: " << is_dragged << ", mouse pos: (" << mouse_x << ", " << mouse_y << "), handle pos_x: (" << handle.position.x << ")" << std::boolalpha << ", is_mouse_on_handle: " << is_mouse_on_handle(mouse_x, mouse_y) << std::endl;
}
