#include "textbox.h"

Textbox::Textbox(SDL_Color text_color, SDL_Renderer* renderer)
	: namebox("img/namebox.png", 500, 511, renderer), textbox("img/textbox.png", 450, 550, renderer), text("", text_color, "fonts/Aller_Rg.ttf", 20, 480, 580, renderer, 770)
{

}

void Textbox::draw(SDL_Renderer* renderer)
{
	this->namebox.draw(renderer);
	this->textbox.draw(renderer);
	this->text.draw(renderer);
}

void Textbox::update(Uint64& time_step)
{
	this->text.update(time_step);
}
