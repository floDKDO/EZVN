#include "GUI/texttoggle.h"
#include "constants.h"

#include <iostream>

const unsigned int TextToggle::index_rect_texttoggle_ = 0;

TextToggle::TextToggle(const std::string text, const int x, const int y, const bool is_checked, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: Checkable(is_checked, renderer), 
	text_(text, constants::texttoggle_unchecked_color_, constants::texttoggle_font_, constants::texttoggle_text_size_, x, y, renderer)
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
	if(is_checked_)
	{
		text_.change_color(constants::texttoggle_checked_color_);
	}
	else
	{
		if(state_ == State::NORMAL)
		{
			text_.change_color(constants::texttoggle_unchecked_color_);
		}
		else if(state_ == State::SELECTED || state_ == State::CLICKED)
		{
			text_.change_color(constants::texttoggle_selected_color_);
		}
	}
}

SDL_Rect TextToggle::get_rect() const
{
	return text_.position_;
}