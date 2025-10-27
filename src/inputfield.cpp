#include "inputfield.h"

Inputfield::Inputfield(const std::string path, const SDL_Color color_normal, unsigned int character_limit, const int x, const int y, SDL_Renderer* renderer, std::function<void(Ui* ui)> callback_function)
	: text("", color_normal, "fonts/Aller_Rg.ttf", 50, x, y, renderer),
	  normal(path, x, y, renderer), color_normal(color_normal), 
	  character_limit(character_limit),
	  text_caret("|", color_normal, "fonts/Aller_Rg.ttf", 50, x, y, renderer), 
	  text_placeholder("...", color_normal, "fonts/Aller_Rg.ttf", 50, x, y, renderer), 
	  is_editing(false), is_writing(false)
{
	this->callback_function = callback_function;
	this->position = this->normal.position;
	SDL_SetTextInputRect(&(this->position));
}

void Inputfield::on_pointer_up()
{
	Ui::on_pointer_up();
	this->is_editing = !this->is_editing;
}

void Inputfield::on_input_pressed(const SDL_Event& e)
{
	this->is_writing = true;
	Ui::on_input_pressed(e);
	if(e.key.keysym.sym == SDLK_BACKSPACE)
	{
		if(this->is_editing && !text.text.empty())
		{
			text.text.pop_back();
		}
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
	else this->text_caret.position.x = this->position.x + this->text.position.w; 

	if(this->is_editing)
	{
		this->text_caret.draw(renderer);
	}
}

void Inputfield::update(Uint64& time_step)
{
	this->text.update(time_step);
	if(SDL_GetTicks() - time_step > 500)
	{
		this->text_caret.show();
	}
	if(SDL_GetTicks() - time_step > 1000 && !this->is_writing) //do not hide the caret when writing/deleting
	{
		this->text_caret.hide();
		time_step = SDL_GetTicks();
	}
}

void Inputfield::on_typing(const SDL_Event& e)
{
	if(this->is_editing && this->text.text.length() <= this->character_limit)
	{
		this->text.text += e.text.text;
	}
}

void Inputfield::set_character_limit(unsigned int character_limit)
{
	this->character_limit = character_limit;
}