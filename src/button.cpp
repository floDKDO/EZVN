#include "button.h"

Button::Button(std::string path_normal, int x, int y, SDL_Renderer* renderer, void(*function_ptr)(void))
	:function_ptr(function_ptr)
{
	this->normal_image = IMG_LoadTexture(renderer, path_normal.c_str());

	int w, h;
	SDL_QueryTexture(this->normal_image, nullptr, nullptr, &w, &h);
	this->position = {x, y, w, h};
}

void Button::on_pointer_down()
{
	this->state = State::CLICKED;
}

void Button::on_pointer_enter()
{
	this->state = State::SELECTED;
	this->select_sound.play_sound();
}

void Button::on_pointer_exit()
{
	this->state = State::NORMAL;
}

void Button::on_pointer_up()
{
	this->state = State::SELECTED;
	function_ptr();
}

void Button::on_key_pressed()
{

}

void Button::on_key_released()
{

}

void Button::draw(SDL_Renderer* renderer)
{
	if(this->state == State::NORMAL)
		SDL_RenderCopy(renderer, this->normal_image, nullptr, &position);
}
