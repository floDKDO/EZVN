#include "button.h"

Button::Button(const std::string path_normal, const std::string path_selected, const std::string path_clicked, const int x, const int y, SDL_Renderer* renderer, std::function<void(Ui* ui)> callback_function)
	: normal(path_normal, x, y, renderer), selected(path_selected, x, y, renderer), clicked(path_clicked, x, y, renderer)
{
	this->callback_function = callback_function;
	this->position = this->normal.position;
	this->pointer_on_ui_when_pointer_up = true;
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

void Button::update(Uint64 time_step)
{
	(void)time_step;
}