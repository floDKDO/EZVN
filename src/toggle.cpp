#include "toggle.h"

Toggle::Toggle(const std::string path_normal, const std::string path_selected, const std::string path_clicked, const std::string path_checked, const int x, const int y, bool is_checked, SDL_Renderer* renderer, void(*callback_function)(Ui* ui))
	: normal(path_normal, x, y, renderer), selected(path_selected, x, y, renderer), clicked(path_clicked, x, y, renderer), 
	  checked(path_checked, x, y, renderer), is_checked(is_checked) 
{
	this->callback_function = callback_function;
	this->position = this->normal.position;
}

void Toggle::on_pointer_up()
{
	Ui::on_pointer_up();
	this->is_checked = !this->is_checked;
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

void Toggle::update(Uint64& time_step)
{
	(void)time_step;
}
