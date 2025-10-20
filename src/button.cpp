#include "button.h"

#include <SDL2/SDL_image.h>
#include <iostream>

Button::Button(const std::string path_normal, const std::string path_selected, const std::string path_clicked, const int x, const int y, SDL_Renderer* renderer, void(*callback_function)(Ui* ui))
	: normal(path_normal, x, y, renderer), selected(path_selected, x, y, renderer), clicked(path_clicked, x, y, renderer)
{
	this->callback_function = callback_function;
	this->position = this->normal.position;
}

void Button::on_pointer_down()
{
	this->state = State::CLICKED;
}

void Button::on_pointer_enter()
{
	this->state = State::SELECTED;
	if(!this->is_selected_sound_played)
	{
		this->select_sound.play_sound();
		this->is_selected_sound_played = true;
	}
}

void Button::on_pointer_exit()
{
	this->state = State::NORMAL;
	this->is_selected_sound_played = false;
}

void Button::on_pointer_up()
{
	this->state = State::SELECTED;
	callback_function(this);
	this->click_sound.play_sound();
}

void Button::on_key_pressed(const SDL_Event& e)
{
	switch(e.key.keysym.sym)
	{
		case SDLK_UP:
			break;

		case SDLK_DOWN:
			break;

		case SDLK_LEFT:
			break;

		case SDLK_RIGHT:
			break;

		case SDLK_RETURN:
			this->on_pointer_down();
			break;

		default:
			break;
	}
}

void Button::on_key_released(const SDL_Event& e)
{
	switch(e.key.keysym.sym)
	{
		case SDLK_UP:
			break;

		case SDLK_DOWN:
			break;

		case SDLK_LEFT:
			break;

		case SDLK_RIGHT:
			break;

		case SDLK_RETURN:
			this->on_pointer_up(); 
			break;

		default:
			break;
	}
}

void Button::draw(SDL_Renderer* renderer)
{
	if(this->state == State::NORMAL)
		this->normal.draw(renderer);
	else if(this->state == State::SELECTED)
		this->selected.draw(renderer);
	else if(this->state == State::CLICKED)
		this->clicked.draw(renderer);
}

void Button::update(Uint64& timeStep) //TODO : inutilisée
{

}