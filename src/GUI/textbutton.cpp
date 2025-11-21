#include "GUI/textbutton.h"

#include <iostream>

TextButton::TextButton(const std::string text, const SDL_Color color_normal, const SDL_Color color_selected, const SDL_Color color_clicked, const int x, const int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: Ui(renderer), text_(text, color_normal, "fonts/Aller_Rg.ttf", 50, x, y, renderer),
	  color_normal_(color_normal), color_selected_(color_selected), color_clicked_(color_clicked)
{
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = true;
}

void TextButton::draw(sdl::Renderer& renderer)
{
	text_.draw(renderer);
}

void TextButton::update(Uint64 time_step)
{
	(void)time_step;
	if(state_ == State::NORMAL)
		text_.change_color(color_normal_);
	else if(state_ == State::SELECTED)
		text_.change_color(color_selected_);
	else if(state_ == State::CLICKED)
		text_.change_color(color_clicked_);
}

SDL_Rect TextButton::get_rect() const
{
	return text_.position_;
}