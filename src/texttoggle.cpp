#include "texttoggle.h"

TextToggle::TextToggle(const std::string text, SDL_Color color_unchecked, SDL_Color color_selected, SDL_Color color_checked, const int x, const int y, bool is_checked, SDL_Renderer* renderer, std::function<void(Ui* ui)> callback_function)
	: text(text, color_unchecked, "fonts/Aller_Rg.ttf", 50, x, y, renderer), 
	  color_unchecked(color_unchecked), color_selected(color_selected), color_checked(color_checked),
	  is_checked(is_checked)
{
	this->callback_function = callback_function;
	this->position = this->text.position;
}

void TextToggle::on_pointer_up()
{
	Ui::on_pointer_up();
	this->is_checked = !this->is_checked;
}

void TextToggle::draw(SDL_Renderer* renderer)
{
	this->text.draw(renderer);
}

void TextToggle::update(Uint64& time_step)
{
	(void)time_step;
	if(this->is_checked)
	{
		this->text.change_color(this->color_checked);
	}
	else
	{
		if(this->state == State::NORMAL)
			this->text.change_color(this->color_unchecked);
		else if(this->state == State::SELECTED || this->state == State::CLICKED)
			this->text.change_color(this->color_selected);
	}
}
