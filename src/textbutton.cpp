#include "textbutton.h"

#include <iostream>

TextButton::TextButton(const std::string text, const SDL_Color color_normal, const SDL_Color color_selected, const SDL_Color color_clicked, const int x, const int y, SDL_Renderer* renderer, std::function<void(Ui* ui)> callback_function)
	: text(text, color_normal, "fonts/Aller_Rg.ttf", 50, x, y, renderer), 
	  color_normal(color_normal), color_selected(color_selected), color_clicked(color_clicked)
{
	this->callback_function = callback_function;
	this->position = this->text.position;
	this->pointer_on_ui_when_pointer_up = true;
	this->renderer = renderer;
}

void TextButton::draw(SDL_Renderer* renderer)
{
	this->text.draw(renderer);
}

void TextButton::update(Uint64 time_step)
{
	(void)time_step;
	if(this->state == State::NORMAL)
		this->text.change_color(this->color_normal);
	else if(this->state == State::SELECTED)
		this->text.change_color(this->color_selected);
	else if(this->state == State::CLICKED)
		this->text.change_color(this->color_clicked);
}

std::vector<SDL_Rect> TextButton::get_bounds() const
{
	return {this->text.position};
}