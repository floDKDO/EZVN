#include "texttoggle.h"
#include "texttogglegroup.h"

#include <iostream>

TextToggle::TextToggle(const std::string text, SDL_Color color_unchecked, SDL_Color color_selected, SDL_Color color_checked, const int x, const int y, bool is_checked, SDL_Renderer* renderer, std::function<void(Ui* ui)> callback_function)
	: text(text, color_unchecked, "fonts/Aller_Rg.ttf", 50, x, y, renderer), 
	  color_unchecked(color_unchecked), color_selected(color_selected), color_checked(color_checked),
	  is_checked(is_checked), previous_checked(is_checked)
{
	this->callback_function = callback_function;
	this->pointer_on_ui_when_pointer_up = true;
	this->renderer = renderer;
}

void TextToggle::on_pointer_up_hook_end()
{
	this->previous_checked = this->is_checked;
	this->is_checked = !this->is_checked;
}

void TextToggle::on_enter_released_hook_end()
{
	this->previous_checked = this->is_checked;
	this->is_checked = !this->is_checked;
}

void TextToggle::on_pointer_up()
{
	Ui::on_pointer_up();
	if(this->texttoggle_group != nullptr)
	{
		this->texttoggle_group->on_click(this);
	}
}

void TextToggle::on_enter_released()
{
	Ui::on_enter_released();
	if(this->texttoggle_group != nullptr)
	{
		this->texttoggle_group->on_click(this);
	}
}

void TextToggle::draw(SDL_Renderer* renderer)
{
	this->text.draw(renderer);
}

void TextToggle::update(Uint64 time_step)
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

SDL_Rect TextToggle::get_rect() const
{
	return this->text.position;
}