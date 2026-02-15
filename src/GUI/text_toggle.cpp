#include "GUI/text_toggle.h"
#include "constants.h"

#include <iostream>

const unsigned int TextToggle::index_rect_texttoggle_ = 0;

TextToggle::TextToggle(std::string_view text, int x, int y, bool is_checked, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function, Kind texttoggle_kind)
	: Checkable(is_checked, renderer),
	text_(text, is_checked ? constants::texttoggle_normal_checked_color_ : constants::texttoggle_normal_unchecked_color_, constants::texttoggle_font_, texttoggle_kind == Kind::ON_TEXTBOX ? constants::textbox_ui_elements_text_size_ : constants::texttoggle_text_size_, x, y, renderer)
{
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = true;

	rect_ = text_.position_;
}

void TextToggle::draw(sdl::Renderer& renderer)
{
	text_.draw(renderer);
}

void TextToggle::update()
{
	text_.update(); //doit être placé en premier pour que le premier UI SELECTED dans un menu ait la bonne couleur

	if(state_ == State::NORMAL)
	{
		if(is_checked_)
		{
			if(text_.color_.r_ != constants::texttoggle_normal_checked_color_.r_
			|| text_.color_.g_ != constants::texttoggle_normal_checked_color_.g_
			|| text_.color_.b_ != constants::texttoggle_normal_checked_color_.b_
			|| text_.color_.a_ != constants::texttoggle_normal_checked_color_.a_)
			{
				text_.change_color(constants::texttoggle_normal_checked_color_);
			}
		}
		else
		{
			if(text_.color_.r_ != constants::texttoggle_normal_unchecked_color_.r_
			|| text_.color_.g_ != constants::texttoggle_normal_unchecked_color_.g_
			|| text_.color_.b_ != constants::texttoggle_normal_unchecked_color_.b_
			|| text_.color_.a_ != constants::texttoggle_normal_unchecked_color_.a_)
			{
				text_.change_color(constants::texttoggle_normal_unchecked_color_);
			}
		}
	}
	else if(state_ == State::SELECTED)
	{
		if(is_checked_)
		{
			if(text_.color_.r_ != constants::texttoggle_selected_checked_color_.r_
			|| text_.color_.g_ != constants::texttoggle_selected_checked_color_.g_
			|| text_.color_.b_ != constants::texttoggle_selected_checked_color_.b_
			|| text_.color_.a_ != constants::texttoggle_selected_checked_color_.a_)
			{
				text_.change_color(constants::texttoggle_selected_checked_color_);
			}
		}
		else
		{
			if(text_.color_.r_ != constants::texttoggle_selected_unchecked_color_.r_
			|| text_.color_.g_ != constants::texttoggle_selected_unchecked_color_.g_
			|| text_.color_.b_ != constants::texttoggle_selected_unchecked_color_.b_
			|| text_.color_.a_ != constants::texttoggle_selected_unchecked_color_.a_)
			{
				text_.change_color(constants::texttoggle_selected_unchecked_color_);
			}
		}
	}
	else if(state_ == State::PRESSED)
	{
		if(is_checked_)
		{
			if(text_.color_.r_ != constants::texttoggle_pressed_checked_color_.r_
			|| text_.color_.g_ != constants::texttoggle_pressed_checked_color_.g_
			|| text_.color_.b_ != constants::texttoggle_pressed_checked_color_.b_
			|| text_.color_.a_ != constants::texttoggle_pressed_checked_color_.a_)
			{
				text_.change_color(constants::texttoggle_pressed_checked_color_);
			}
		}
		else
		{
			if(text_.color_.r_ != constants::texttoggle_pressed_unchecked_color_.r_
			|| text_.color_.g_ != constants::texttoggle_pressed_unchecked_color_.g_
			|| text_.color_.b_ != constants::texttoggle_pressed_unchecked_color_.b_
			|| text_.color_.a_ != constants::texttoggle_pressed_unchecked_color_.a_)
			{
				text_.change_color(constants::texttoggle_pressed_unchecked_color_);
			}
		}
	}
}

void TextToggle::change_position(int x, int y)
{
	text_.position_.x = x;
	text_.position_.y = y;

	rect_ = text_.position_;
}