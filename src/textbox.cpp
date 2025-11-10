#include "textbox.h"

Textbox::Textbox(SDL_Color text_color, SDL_Renderer* renderer)
	: namebox("img/namebox.png", 500, 511, renderer), textbox("img/textbox.png", 450, 550, renderer), text("", text_color, "fonts/Aller_Rg.ttf", 20, 480, 580, renderer, 770), text_name_box("Monika", text_color, "fonts/Aller_Rg.ttf", 30, 520, 510, renderer)
{

}

//TODO : créer une méthode change_text qui prend deux paramètres (texte, et qui parle = default parameter="")

void Textbox::draw(SDL_Renderer* renderer)
{
	this->namebox.draw(renderer);
	this->textbox.draw(renderer);
	this->text.draw(renderer);
	if(!this->text_name_box.text.empty())
	{
		this->text_name_box.draw(renderer);
	}
}

void Textbox::update(Uint64& time_step)
{
	this->text.update(time_step);
	if(!this->text_name_box.text.empty())
	{
		this->text_name_box.update(time_step);
	}
}
