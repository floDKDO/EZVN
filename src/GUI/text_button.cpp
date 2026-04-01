#include "GUI/text_button.h"
#include "constants.h"

#include <iostream>

TextButton::TextButton(std::string_view text, int x, int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function, Kind textbutton_kind)
	: UiWidget(renderer),
	text_(text, constants::textbutton_normal_color_, constants::textbutton_font_, textbutton_kind == Kind::ON_TEXTBOX ? constants::textbox_ui_elements_text_size_ : textbutton_kind == Kind::ON_FRAME ? constants::confirmationpopup_text_size_ : constants::textbutton_text_size_, x, y, renderer)
{
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = true;

	rect_ = text_.position_;
}

void TextButton::draw(sdl::Renderer& renderer)
{
	text_.draw(renderer);
}

void TextButton::update()
{
	text_.update(); //doit être placé en premier pour que le premier UI SELECTED dans un menu ait la bonne couleur

	if(state_ == State::NORMAL)
	{
		if(text_.color_.r_ != constants::textbutton_normal_color_.r_ 
		|| text_.color_.g_ != constants::textbutton_normal_color_.g_ 
		|| text_.color_.b_ != constants::textbutton_normal_color_.b_ 
		|| text_.color_.a_ != constants::textbutton_normal_color_.a_)
		{
			text_.change_color(constants::textbutton_normal_color_);
		}
	}
	else if(state_ == State::SELECTED)
	{
		if(text_.color_.r_ != constants::textbutton_selected_color_.r_
		|| text_.color_.g_ != constants::textbutton_selected_color_.g_
		|| text_.color_.b_ != constants::textbutton_selected_color_.b_
		|| text_.color_.a_ != constants::textbutton_selected_color_.a_)
		{
			text_.change_color(constants::textbutton_selected_color_);
		}
	}
	else if(state_ == State::PRESSED)
	{
		if(text_.color_.r_ != constants::textbutton_pressed_color_.r_
		|| text_.color_.g_ != constants::textbutton_pressed_color_.g_
		|| text_.color_.b_ != constants::textbutton_pressed_color_.b_
		|| text_.color_.a_ != constants::textbutton_pressed_color_.a_)
		{
			text_.change_color(constants::textbutton_pressed_color_);
		}
	}
}

void TextButton::change_position(int x, int y)
{
	text_.position_.x = x;
	text_.position_.y = y;

	rect_ = text_.position_;
}