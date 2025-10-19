#include "inputfield.h"

#include <SDL2/SDL_image.h>
#include <iostream>

Inputfield::Inputfield(std::string path, SDL_Color color, int x, int y, SDL_Renderer* renderer, void(*function_ptr)(std::string text))
	: character_limit(7),/*TODO : 7 ??*/ color(color), text("", color, "fonts/Aller_Rg.ttf", x, y, renderer), text_caret("|", color, "fonts/Aller_Rg.ttf", x, y, renderer), text_placeholder("...", color, "fonts/Aller_Rg.ttf", x, y, renderer), is_editing(false), is_writing(false), function_ptr(function_ptr)
{
	this->image = IMG_LoadTexture(renderer, path.c_str());

	int w, h;
	SDL_QueryTexture(this->image, nullptr, nullptr, &w, &h);
	this->position = {x, y, w, h};

	SDL_SetTextInputRect(&(this->position));

	SDL_SetTextureBlendMode(this->text_caret.texture, SDL_BLENDMODE_BLEND);
}

Inputfield::~Inputfield()
{
	SDL_DestroyTexture(this->image);
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
	//TODO : créer une fonction on_click ?
	this->state = State::SELECTED;
	function_ptr(this->text.text); 
	this->click_sound.play_sound();
	this->is_editing = !this->is_editing;
}

void Inputfield::on_key_pressed(SDL_Event e)
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

void Inputfield::on_key_released(SDL_Event e)
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
			this->on_pointer_up(); //TODO : créer une fonction on_click ?
			break;

		default:
			break;
	}
}

void Inputfield::on_input(SDL_Event e)
{
	if(this->is_editing)
	{
		text.text += e.text.text;
	}
}

void Inputfield::handle_events(SDL_Event e)
{
	Ui::handle_events(e);

	if(e.type == SDL_TEXTINPUT)
	{
		this->on_input(e);
	}
}


void Inputfield::draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, this->image, nullptr, &(this->position));
	text.draw(renderer);
	text.edit_text(text.text, renderer);

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
		//std::cout << this->text_caret.position.x << " et " << this->text.position.w << std::endl;
	}
}

void Inputfield::update(Uint32& timeStep)
{
	//toutes les demis secondes, alterner affichage et non affichage
	if(SDL_GetTicks() - timeStep > 500)
	{
		this->text_caret.color.a = 255;
		SDL_SetTextureAlphaMod(this->text_caret.texture, this->text_caret.color.a); 
	}
	if(SDL_GetTicks() - timeStep > 1000 && !this->is_writing) 
	{
		this->text_caret.color.a = 0;
		SDL_SetTextureAlphaMod(this->text_caret.texture, this->text_caret.color.a);
		timeStep = SDL_GetTicks();
	}
}