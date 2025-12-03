#include "GUI/checkbox.h"
#include "constants.h"

#include <iostream>

const unsigned int Checkbox::index_rect_checkbox_ = 0;

Checkbox::Checkbox(const std::string text, const int x, const int y, const bool is_checked, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: Checkable(is_checked, renderer), normal_(constants::checkbox_normal_, x, y, renderer), selected_(constants::checkbox_selected_, x, y, renderer), clicked_(constants::checkbox_clicked_, x, y, renderer),
	  check_mark_(constants::checkbox_check_mark_, x, y, renderer), 
	  text_(text, constants::checkbox_text_color_, constants::checkbox_font_, constants::checkbox_text_size_, x + normal_.position_.w + constants::checkbox_text_x_delta_, y + (normal_.position_.h / 2), renderer)
{
	text_.position_.y -= text_.get_height_text() / 2;
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

	text_.draw(renderer);
}

void Checkbox::update()
{

}

SDL_Rect Checkbox::get_rect() const
{
	return normal_.position_;
}