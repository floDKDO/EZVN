#include "textbutton.h"
#include <iostream>

//TODO : trop de code dupliqu� avec la classe Button
TextButton::TextButton(const std::string text, const SDL_Color color_normal, const SDL_Color color_selected, const SDL_Color color_clicked, const int x, const int y, SDL_Renderer* renderer, void(*callback_function)(Ui* ui))
	: text(text, color_normal, "fonts/Aller_Rg.ttf", 50, x, y, renderer), color_normal(color_normal), color_selected(color_selected), color_clicked(color_clicked)
{
	this->callback_function = callback_function;
	this->position = this->text.position;
}


void TextButton::on_pointer_down()
{
	this->state = State::CLICKED;
}


void TextButton::on_pointer_enter()
{
	this->state = State::SELECTED;
	if(!this->is_selected_sound_played)
	{
		this->select_sound.play_sound();
		this->is_selected_sound_played = true;
	}
}

void TextButton::on_pointer_exit()
{
	this->state = State::NORMAL;
	this->is_selected_sound_played = false;
}

void TextButton::on_pointer_up() //<=> on_click (l'action se lance quand le clic est relach�)
{
	this->state = State::SELECTED;
	callback_function(this);
	this->click_sound.play_sound();
}


//TODO : abstraire le fait que �a soit une touche du clavier ou la manette dans les if
void TextButton::on_key_pressed(const SDL_Event& e)
{
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
			this->on_pointer_down();
			break;

		default:
			break;
	}
}

void TextButton::on_key_released(const SDL_Event& e)
{
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


void TextButton::draw(SDL_Renderer* renderer)
{
	if(this->state == State::NORMAL)
		this->text.color = this->color_normal;
	else if(this->state == State::SELECTED)
		this->text.color = this->color_selected;
	else if(this->state == State::CLICKED)
		this->text.color = this->color_clicked;

	this->text.draw(renderer);
}

void TextButton::update(Uint64& timeStep) //TODO : inutilis�e
{

}