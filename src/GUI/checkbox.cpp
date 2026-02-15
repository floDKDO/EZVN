#include "GUI/checkbox.h"
#include "constants.h"

#include <iostream>

const unsigned int Checkbox::index_rect_checkbox_ = 0;

Checkbox::Checkbox(std::string_view text, int x, int y, bool is_checked, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: Checkable(is_checked, renderer), 
	  normal_(constants::checkbox_normal_, x, y, renderer), 
	  selected_(constants::checkbox_selected_, x, y, renderer), 
	  pressed_(constants::checkbox_pressed_, x, y, renderer),
	  check_mark_(constants::checkbox_check_mark_, x, y, renderer), 
	  text_(text, constants::checkbox_text_color_, constants::checkbox_font_, constants::checkbox_text_size_, x + normal_.position_.w + constants::checkbox_text_x_delta_, y + (normal_.position_.h / 2), renderer)
{
	text_.position_.y -= text_.get_height_text() / 2;
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = true;

	rect_ = normal_.position_;
}

void Checkbox::draw(sdl::Renderer& renderer)
{
	if(state_ == State::NORMAL)
	{
		normal_.draw(renderer);
	}
	else if(state_ == State::SELECTED)
	{
		selected_.draw(renderer);
	}
	else if(state_ == State::PRESSED)
	{
		pressed_.draw(renderer);
	}

	if(is_checked_)
	{
		check_mark_.draw(renderer);
	}

	text_.draw(renderer);
}

void Checkbox::update()
{
	normal_.update();
	selected_.update();
	pressed_.update();
	check_mark_.update();
	text_.update();
}

void Checkbox::change_position(int x, int y)
{
	normal_.position_.x = x;
	normal_.position_.y = y;

	selected_.position_.x = x;
	selected_.position_.y = y;

	pressed_.position_.x = x;
	pressed_.position_.y = y;

	check_mark_.position_.x = x;
	check_mark_.position_.y = y;

	text_.position_.x = x + normal_.position_.w + constants::checkbox_text_x_delta_;
	text_.position_.y = y + (normal_.position_.h / 2);

	rect_ = normal_.position_;
}