#include "inputfield.h"

#include <iostream>

Inputfield::Inputfield(const std::string path, const SDL_Color color_normal, unsigned int character_limit, const int x, const int y, SDL_Renderer* renderer, std::function<void(Ui* ui)> callback_function)
	: text("", color_normal, "fonts/Aller_Rg.ttf", 50, x+7, y, renderer),
	  normal(path, x, y, renderer), color_normal(color_normal), 
	  character_limit(character_limit),
	  text_caret("|", color_normal, "fonts/Aller_Rg.ttf", 50, x, y, renderer), 
	  text_placeholder("...", color_normal, "fonts/Aller_Rg.ttf", 50, x, y, renderer), 
	  is_editing(false), is_writing(false), index_caret(0), offset_caret(0)
{
	this->callback_function = callback_function;
	this->position = this->normal.position;
	SDL_SetTextInputRect(&(this->position));

	this->pointer_on_ui_when_pointer_up = true;
	this->renderer = renderer;
}

void Inputfield::on_pointer_up()
{
	Ui::on_pointer_up();
	this->is_editing = !this->is_editing;
}

void Inputfield::on_left_pressed()
{
	if(this->is_editing)
	{
		if(this->index_caret > 0)
		{
			int current_char_width = this->text.get_width_one_char(this->text.text[this->index_caret - 1]);
			this->offset_caret += -current_char_width;
			this->index_caret -= 1;
		}
	}
	else
	{
		Ui::on_left_pressed();
	}
}

void Inputfield::on_right_pressed()
{
	if(this->is_editing)
	{
		if(this->index_caret < this->character_limit && this->index_caret < this->text.text.length())
		{
			int current_char_width = this->text.get_width_one_char(this->text.text[this->index_caret]);
			this->offset_caret += current_char_width;
			this->index_caret += 1;
		}
	}
	else
	{
		Ui::on_right_pressed();
	}
}

void Inputfield::on_enter_pressed()
{
	if(this->lock && this->state == State::SELECTED)
	{
		this->state = State::CLICKED;
		this->lock = false;
		this->is_editing = !this->is_editing;
	}
}

void Inputfield::on_backspace_pressed()
{
	if(this->is_editing && !text.text.empty())
	{
		if(this->index_caret > 0)
		{
			this->text.text.erase(this->index_caret - 1, 1);
			this->index_caret -= 1;
		}
	}
}

void Inputfield::on_delete_pressed()
{
	if(this->is_editing && !text.text.empty())
	{
		if(this->index_caret < this->text.text.length())
		{
			int current_char_width = this->text.get_width_one_char(this->text.text[this->index_caret]);
			this->offset_caret += current_char_width;
			this->text.text.erase(this->index_caret, 1);
		}
	}
}

void Inputfield::on_input_pressed(const SDL_Event& e)
{
	Ui::on_input_pressed(e);

	if(this->is_editing)
	{
		this->is_writing = true;
	}

	if(e.key.keysym.sym == SDLK_BACKSPACE)
	{
		this->on_backspace_pressed();
	}
	else if(e.key.keysym.sym == SDLK_DELETE)
	{
		this->on_delete_pressed();
	}
}

void Inputfield::on_input_released(const SDL_Event& e)
{
	this->is_writing = false;
	Ui::on_input_released(e);
}

void Inputfield::handle_events(const SDL_Event& e)
{
	Ui::handle_events(e);

	if(e.type == SDL_TEXTINPUT)
	{
		this->on_typing(e);
	}
}

void Inputfield::draw(SDL_Renderer* renderer)
{
	normal.draw(renderer);
	text.draw(renderer);

	if(text.text.empty()) 
	{
		if(!this->is_editing)
		{
			text_placeholder.draw(renderer);
		}
		this->text_caret.position.x = this->position.x; 
	}
	else this->text_caret.position.x = this->position.x + this->text.position.w + this->offset_caret;

	if(this->is_editing)
	{
		this->text_caret.draw(renderer);
	}
}

void Inputfield::update(Uint64 time_step)
{
	this->text.update(time_step);

	if(time_step - this->last_time > 500)
	{
		this->text_caret.show();
	}
	if(time_step - this->last_time > 1000 && !this->is_writing) //do not hide the caret when writing/deleting
	{
		this->text_caret.hide();
		last_time = SDL_GetTicks64();
	}
}

void Inputfield::on_typing(const SDL_Event& e)
{
	if(this->is_editing && this->text.text.length() < this->character_limit)
	{
		this->text.text.insert(this->text.text.begin() + this->index_caret, e.text.text[0]);
		this->index_caret += 1;
	}
}

void Inputfield::set_character_limit(unsigned int character_limit)
{
	this->character_limit = character_limit;
}

std::vector<SDL_Rect> Inputfield::get_bounds() const
{
	return {this->normal.position};
}