#include "GUI/texttoggle.h"

#include <iostream>

TextToggle::TextToggle(const std::string text, SDL_Color color_unchecked, SDL_Color color_selected, SDL_Color color_checked, const int x, const int y, bool is_checked, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: Checkable(is_checked, renderer), text_(text, color_unchecked, "fonts/Aller_Rg.ttf", 50, x, y, renderer),
	  color_unchecked_(color_unchecked), color_selected_(color_selected), color_checked_(color_checked)
{
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = true;
}

void TextToggle::draw(sdl::Renderer& renderer)
{
	text_.draw(renderer);
}

void TextToggle::update(Uint64 time_step)
{
	(void)time_step;
	if(is_checked_)
	{
		text_.change_color(color_checked_);
	}
	else
	{
		if(state_ == State::NORMAL)
		{
			text_.change_color(color_unchecked_);
		}
		else if(state_ == State::SELECTED || state_ == State::CLICKED)
		{
			text_.change_color(color_selected_);
		}
	}
}

SDL_Rect TextToggle::get_rect() const
{
	return text_.position_;
}