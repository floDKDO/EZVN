#include "toggle.h"

#include <SDL2/SDL_image.h>

Toggle::Toggle(const std::string path_normal, const std::string path_selected, const std::string path_clicked, const std::string path_checked, const int x, const int y, bool is_checked, SDL_Renderer* renderer, void(*callback_function)(Ui* ui))
	: normal(path_normal, x, y, renderer), selected(path_selected, x, y, renderer), clicked(path_clicked, x, y, renderer), checked(path_checked, x, y, renderer), is_checked(is_checked) 
{
	this->callback_function = callback_function;
	this->position = this->normal.position;
}

void Toggle::on_pointer_down()
{
	this->state = State::CLICKED;
}

void Toggle::on_pointer_enter()
{
	this->state = State::SELECTED;
	if(!this->is_selected_sound_played)
	{
		this->select_sound.play_sound();
		this->is_selected_sound_played = true;
	}
}

void Toggle::on_pointer_exit()
{
	this->state = State::NORMAL;
	this->is_selected_sound_played = false;
}

void Toggle::on_pointer_up()
{
	this->state = State::SELECTED;
	callback_function(this);
	this->click_sound.play_sound();
	this->is_checked = !this->is_checked;
}

void Toggle::on_key_pressed(const SDL_Event& e)
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

void Toggle::on_key_released(const SDL_Event& e)
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

void Toggle::draw(SDL_Renderer* renderer)
{
	if(this->state == State::NORMAL)
		this->normal.draw(renderer);
	else if(this->state == State::SELECTED)
		this->selected.draw(renderer);
	else if(this->state == State::CLICKED)
		this->clicked.draw(renderer);

	if(this->is_checked)
		this->checked.draw(renderer);
}

void Toggle::update(Uint64& timeStep) //TODO : inutilisée
{

}
