#include "textbox.h"

#include <iostream>

Textbox::Textbox(SDL_Color text_color, SDL_Renderer* renderer)
	: namebox("img/gui/namebox.png", 500, 511, renderer), 
	textbox("img/gui/textbox.png", 450, 550, renderer), 
	text("", text_color, "fonts/Aller_Rg.ttf", 20, 480, 580, renderer, 770), 
	text_name_box("", text_color, "fonts/Aller_Rg.ttf", 30, 520, 513, renderer), //TODO : center le texte dans la Namebox
	triangle("img/gui/triangle_textbox.png", 480, 650, renderer),
	current_speaker("") 
{

}

void Textbox::show_new_dialogue(std::string new_dialogue, std::string speaker)
{
	if(this->text.is_finished)
	{
		this->current_speaker = speaker;
		this->text_name_box.text = this->current_speaker;

		this->text_name_box.position.x = this->namebox.position.x + ((this->namebox.position.w - this->text_name_box.get_width_text()) / 2);

		this->text.is_finished = false;
		this->text.index_dialogue = 0;
		this->text.text = "";
		this->text.text_dialogue = "";
		this->text.text = new_dialogue;

		if(!this->current_speaker.empty())
		{
			this->text.text.insert(0, "\"");
			this->text.text.append("\"");
		}
	}
}

void Textbox::draw(SDL_Renderer* renderer)
{
	if(!this->current_speaker.empty())
	{
		this->namebox.draw(renderer);
		if(!this->text_name_box.text.empty()) //TODO : cas à vérifier (pas utile car redondant ??)
		{
			this->text_name_box.draw(renderer);
		}
	}
	this->textbox.draw(renderer);
	this->text.draw(renderer);
	if(this->text.is_finished)
	{
		this->triangle.draw(renderer);
	}
}

void Textbox::update(Uint64 time_step) 
{
	this->text.update(time_step);
	if(!this->text_name_box.text.empty())
	{
		this->text_name_box.update(time_step);
	}
	if(this->text.is_finished)
	{
		if(time_step - this->triangle.last_time > 500)
		{
			this->triangle.show();
		}
		if(time_step - this->triangle.last_time > 1000) 
		{
			this->triangle.hide();
			this->triangle.last_time = SDL_GetTicks64();
		}
	}
}
