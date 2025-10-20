#include "toggle.h"

#include <SDL2/SDL_image.h>

Toggle::Toggle(const std::string path_normal, const std::string path_selected, const std::string path_clicked, const std::string path_checked, const int x, const int y, SDL_Renderer* renderer, void(*function_ptr)(void))
	:function_ptr(function_ptr), is_checked(false) //TODO : pas forcément checked ??
{
	this->normal_image = IMG_LoadTexture(renderer, path_normal.c_str());
	this->selected_image = IMG_LoadTexture(renderer, path_selected.c_str());
	this->clicked_image = IMG_LoadTexture(renderer, path_clicked.c_str());
	this->checked_image = IMG_LoadTexture(renderer, path_checked.c_str());

	int w, h;
	SDL_QueryTexture(this->normal_image, nullptr, nullptr, &w, &h);
	this->position = {x, y, w, h};
}

Toggle::~Toggle()
{
	SDL_DestroyTexture(this->normal_image);
	SDL_DestroyTexture(this->selected_image);
	SDL_DestroyTexture(this->clicked_image);
	SDL_DestroyTexture(this->checked_image);
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
	//TODO : créer une fonction on_click ?
	this->state = State::SELECTED;
	function_ptr();
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
			this->on_pointer_up(); //TODO : créer une fonction on_click ?
			break;

		default:
			break;
	}
}

void Toggle::draw(SDL_Renderer* renderer)
{
	if(this->state == State::NORMAL)
		SDL_RenderCopy(renderer, this->normal_image, nullptr, &position);
	else if(this->state == State::SELECTED)
		SDL_RenderCopy(renderer, this->selected_image, nullptr, &position);
	else if(this->state == State::CLICKED)
		SDL_RenderCopy(renderer, this->clicked_image, nullptr, &position);

	if(this->is_checked)
		SDL_RenderCopy(renderer, this->checked_image, nullptr, &position);
}

void Toggle::update(Uint32& timeStep)
{

}
