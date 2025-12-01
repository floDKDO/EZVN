#include "GUI/checkbox.h"

#include <iostream>

Checkbox::Checkbox(const std::string path_normal, const std::string path_selected, const std::string path_clicked, const std::string path_checked, const int x, const int y, bool is_checked, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: Checkable(is_checked, renderer), normal_(path_normal, x, y, renderer), selected_(path_selected, x, y, renderer), clicked_(path_clicked, x, y, renderer), 
	  check_mark_(path_checked, x, y, renderer)
{
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = true;
}

void Checkbox::draw(sdl::Renderer& renderer)
{
	if(state_ == State::NORMAL)
		normal_.draw(renderer);
	else if(state_ == State::SELECTED)
		selected_.draw(renderer);
	else if(state_ == State::CLICKED)
		clicked_.draw(renderer);

	if(is_checked_)
		check_mark_.draw(renderer);
}

void Checkbox::update()
{

}

SDL_Rect Checkbox::get_rect() const
{
	return normal_.position_;
}