#include "GUI/texttoggle.h"
#include "constants.h"

#include <iostream>

const unsigned int TextToggle::index_rect_texttoggle_ = 0;

TextToggle::TextToggle(const std::string_view text, const int x, const int y, const bool is_checked, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function, const TextToggleKind texttoggle_kind)
	: Checkable(is_checked, renderer), 
	text_(text, is_checked ? constants::texttoggle_normal_checked_color_ : constants::texttoggle_normal_unchecked_color_, constants::texttoggle_font_, texttoggle_kind == TextToggleKind::ON_TEXTBOX ? constants::textbox_ui_elements_text_size_ : constants::texttoggle_text_size_, x, y, renderer)
{
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = true;
}

void TextToggle::draw(sdl::Renderer& renderer)
{
	text_.draw(renderer);
}

void TextToggle::update()
{
	if(state_ == State::NORMAL)
	{
		if(is_checked_)
		{
			text_.change_color(constants::texttoggle_normal_checked_color_);
		}
		else
		{
			text_.change_color(constants::texttoggle_normal_unchecked_color_);
		}
	}
	else if(state_ == State::SELECTED)
	{
		if(is_checked_)
		{
			text_.change_color(constants::texttoggle_selected_checked_color_);
		}
		else
		{
			text_.change_color(constants::texttoggle_selected_unchecked_color_);
		}
	}
	else if(state_ == State::PRESSED)
	{
		if(is_checked_)
		{
			text_.change_color(constants::texttoggle_pressed_checked_color_);
		}
		else
		{
			text_.change_color(constants::texttoggle_pressed_unchecked_color_);
		}
	}
	text_.update();
}

SDL_Rect TextToggle::get_rect() const
{
	return text_.position_;
}