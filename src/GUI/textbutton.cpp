#include "GUI/textbutton.h"
#include "constants.h"

#include <iostream>

const unsigned int TextButton::index_rect_textbutton_ = 0;

TextButton::TextButton(const std::string text, const int x, const int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function, const bool is_on_textbox)
	: Ui(renderer), text_(text, constants::textbutton_text_color_, constants::textbutton_font_, is_on_textbox ? constants::textbox_textbuttons_text_size_ : constants::textbutton_text_size_, x, y, renderer)
{
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = true;
}

void TextButton::draw(sdl::Renderer& renderer)
{
	text_.draw(renderer);
}

void TextButton::update()
{
	if(state_ == State::NORMAL)
		text_.change_color(constants::textbutton_normal_color_);
	else if(state_ == State::SELECTED)
		text_.change_color(constants::textbutton_selected_color_);
	else if(state_ == State::CLICKED)
		text_.change_color(constants::textbutton_clicked_color_);
}

SDL_Rect TextButton::get_rect() const
{
	return text_.position_;
}