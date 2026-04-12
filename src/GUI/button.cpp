#include "GUI/button.h"
#include "constants.h"

#include <iostream>

Button::Button(std::string_view text, int x, int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function, TextPosition text_position)
	: Button(constants::choice_button_normal_, constants::choice_button_selected_, constants::choice_button_selected_, text, x, y, renderer, callback_function, text_position)
{}

Button::Button(std::string_view path_normal, std::string_view path_selected, std::string_view path_pressed, std::string_view text, int x, int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function, TextPosition text_position)
	: UiWidget(renderer), 
	text_(text, constants::button_text_color_, constants::button_font_, constants::choice_button_text_size_, x, y, renderer), //TODO : que pour choice button
	normal_(path_normal, x, y, renderer),
	selected_(path_selected, x, y, renderer),
	pressed_(path_pressed, x, y, renderer), 
	text_position_(text_position)
{
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = true;

	if(text_position == TextPosition::ON)
	{
		text_.position_.x = normal_.position_.x + normal_.position_.w / 2 - text_.get_width_text() / 2;
		text_.position_.y = normal_.position_.y + normal_.position_.h / 2 - text_.get_height_text() / 2;
	}
	else if(text_position == TextPosition::UNDER)
	{
		text_.position_.x = normal_.position_.x + normal_.position_.w / 2 - text_.get_width_text() / 2;
		text_.position_.y = normal_.position_.y + normal_.position_.h;
	}
	rect_ = normal_.position_;
}

void Button::draw(sdl::Renderer& renderer) 
{
	if(state_ == State::NORMAL)
	{
		text_.change_color(Color::from_rgba8(255, 255, 255)); //TODO : que pour choice button
		normal_.draw(renderer);
	}
	else if(state_ == State::SELECTED)
	{
		text_.change_color(Color::from_rgba8(0, 0, 0)); //TODO : que pour choice button
		selected_.draw(renderer);
	}
	else if(state_ == State::PRESSED)
	{
		text_.change_color(Color::from_rgba8(0, 0, 0)); //TODO : que pour choice button
		pressed_.draw(renderer);
	}

	text_.draw(renderer);
}

void Button::update()
{
	text_.update();
	normal_.update();
	selected_.update();
	pressed_.update();
}

void Button::change_position(int x, int y)
{
	if(text_position_ == TextPosition::ON)
	{
		text_.position_.x = x + normal_.position_.w / 2 - text_.get_width_text() / 2;
		text_.position_.y = y + normal_.position_.h / 2 - text_.get_height_text() / 2;
	}
	else if(text_position_ == TextPosition::UNDER)
	{
		text_.position_.x = x + normal_.position_.w / 2 - text_.get_width_text() / 2;
		text_.position_.y = y + normal_.position_.h;
	}

	normal_.position_.x = x;
	normal_.position_.y = y;

	selected_.position_.x = x;
	selected_.position_.y = y;

	pressed_.position_.x = x;
	pressed_.position_.y = y;

	rect_ = normal_.position_;
}