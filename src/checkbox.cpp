#include "checkbox.h"
#include "checkboxgroup.h"

Checkbox::Checkbox(const std::string path_normal, const std::string path_selected, const std::string path_clicked, const std::string path_checked, const int x, const int y, bool is_checked, SDL_Renderer* renderer, std::function<void(Ui* ui)> callback_function)
	: normal(path_normal, x, y, renderer), selected(path_selected, x, y, renderer), clicked(path_clicked, x, y, renderer), 
	  checked(path_checked, x, y, renderer)
{
	this->is_checked = is_checked; //TODO : mettre ces deux lignes dans le constructeur de Checkable ??
	this->previous_checked = is_checked;
	this->callback_function = callback_function;
	this->pointer_on_ui_when_pointer_up = true;
	this->renderer = renderer;
}

void Checkbox::draw(SDL_Renderer* renderer)
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

void Checkbox::update(Uint64 time_step)
{
	(void)time_step;
}

SDL_Rect Checkbox::get_rect() const
{
	return this->normal.position;
}