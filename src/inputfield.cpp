#include "inputfield.h"

#include <SDL2/SDL_image.h>
#include <iostream>

Inputfield::Inputfield(const std::string path, const SDL_Color color_normal, const int x, const int y, SDL_Renderer* renderer, void(*callback_function)(Ui* ui))
	: normal(path, x, y, renderer), character_limit(4),/*TODO : 4 ??*/ color_normal(color_normal), text("", color_normal, "fonts/Aller_Rg.ttf", 50, x, y, renderer), text_caret("|", color_normal, "fonts/Aller_Rg.ttf", 50, x, y, renderer), text_placeholder("...", color_normal, "fonts/Aller_Rg.ttf", 50, x, y, renderer), is_editing(false), is_writing(false)
{
	this->callback_function = callback_function;
	this->position = this->normal.position;
	SDL_SetTextInputRect(&(this->position));
	SDL_SetTextureBlendMode(this->text_caret.texture, SDL_BLENDMODE_BLEND);
}

void Inputfield::on_pointer_enter()
{
	this->state = State::SELECTED;
	if(!this->is_selected_sound_played)
	{
		this->select_sound.play_sound();
		this->is_selected_sound_played = true;
	}
}

void Inputfield::on_pointer_exit()
{
	this->state = State::NORMAL;
	this->is_selected_sound_played = false;
}

void Inputfield::on_pointer_down()
{
	this->state = State::CLICKED;
}

void Inputfield::on_pointer_up()
{
	this->state = State::SELECTED;
	callback_function(this); 
	this->click_sound.play_sound();
	this->is_editing = !this->is_editing;
}

void Inputfield::on_key_pressed(const SDL_Event& e)
{
	this->is_writing = true;
	switch(e.key.keysym.sym)
	{
		case SDLK_UP:
			break;

		case SDLK_DOWN:
			break;

		case SDLK_LEFT:
			break;

		case SDLK_RIGHT:
			break;

		case SDLK_BACKSPACE:
			if(this->is_editing && !text.text.empty())
			{
				text.text.pop_back();
			}
			break;

		case SDLK_RETURN:
			this->on_pointer_down();
			break;

		default:
			break;
	}
}

void Inputfield::on_key_released(const SDL_Event& e)
{
	this->is_writing = false;
	switch(e.key.keysym.sym)
	{
		case SDLK_UP:
			break;

		case SDLK_DOWN:
			break;

		case SDLK_LEFT:
			break;

		case SDLK_RIGHT:
			break;

		case SDLK_RETURN:
			this->on_pointer_up(); 
			break;

		default:
			break;
	}
}

void Inputfield::on_input(const SDL_Event& e)
{
	if(this->is_editing && this->text.text.length() <= this->character_limit)
	{
		text.text += e.text.text;
	}
}

void Inputfield::handle_events(const SDL_Event& e)
{
	Ui::handle_events(e);

	if(e.type == SDL_TEXTINPUT)
	{
		this->on_input(e);
	}
}


void Inputfield::draw(SDL_Renderer* renderer)
{
	normal.draw(renderer);
	text.draw(renderer);
	text.edit_text(renderer); //TODO : utilité ??

	if(text.text.empty() || text.text == " ") //TODO : bizarre...
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

void Inputfield::update(Uint64& timeStep)
{
	if(SDL_GetTicks() - timeStep > 500)
	{
		this->text_caret.color.a = 255;
		SDL_SetTextureAlphaMod(this->text_caret.texture, this->text_caret.color.a); 
	}
	if(SDL_GetTicks() - timeStep > 1000 && !this->is_writing) //do not hide the caret when writing/deleting
	{
		this->text_caret.color.a = 0;
		SDL_SetTextureAlphaMod(this->text_caret.texture, this->text_caret.color.a);
		timeStep = SDL_GetTicks();
	}
}