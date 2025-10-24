#include "button.h"
#include <iostream>

int Button::num = 1;

Button::Button(const std::string path_normal, const std::string path_selected, const std::string path_clicked, const int x, const int y, SDL_Renderer* renderer, void(*callback_function)(Ui* ui))
	: normal(path_normal, x, y, renderer), selected(path_selected, x, y, renderer), clicked(path_clicked, x, y, renderer)
{
	this->name = "button" + std::to_string(this->num);
	std::cout << "CALLED (button), " << &(this->lock) << std::endl;
	this->callback_function = callback_function;
	this->position = this->normal.position;

	this->num += 1;
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

void Button::update(Uint64& time_step)
{
	(void)time_step;
	//std::cout << this->name << " " << int(this->state) << " : " << this->lock << std::endl;
	//std::cout << this->name << ", " << this->lock << std::endl;
}