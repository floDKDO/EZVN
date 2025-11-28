#include "textbox.h"

#include <iostream>

Textbox::Textbox(SDL_Color text_color, sdl::Renderer& renderer)
	: namebox_("img/gui/namebox.png", 0, 0, renderer), 
	textbox_("img/gui/textbox.png", 0, 0, renderer), 
	text_("", text_color, "fonts/Aller_Rg.ttf", 20, 0, 0, renderer, 10), 
	text_name_box_("", text_color, "fonts/Aller_Rg.ttf", 30, 0, 0, renderer), //TODO : paramètres de position inutiles 
	triangle_("img/gui/triangle_textbox.png", 0, 0, renderer),
	current_speaker_("") 
{
	textbox_.set_position((1280 - textbox_.position_.w) / 2, 720 - textbox_.position_.h - 6);
	namebox_.set_position(textbox_.position_.x + 37, textbox_.position_.y - namebox_.position_.h);
	text_name_box_.position_.x = namebox_.position_.x + ((namebox_.position_.w - text_name_box_.get_width_text()) / 2);
	text_name_box_.position_.y = 530;
	text_.position_.x = textbox_.position_.x + 30; //TODO : utiliser get_height_text()
	text_.position_.y = textbox_.position_.y + 25;
	text_.wrap_length_ = textbox_.position_.w - 40;
	triangle_.position_.x = textbox_.position_.x + textbox_.position_.w - 30;
	triangle_.position_.y = textbox_.position_.y + 115;
}

void Textbox::show_new_dialogue(std::string new_dialogue, std::string speaker)
{
	if(text_.is_finished_)
	{
		current_speaker_ = speaker;
		text_name_box_.text_ = current_speaker_;

		text_name_box_.position_.x = namebox_.position_.x + ((namebox_.position_.w - text_name_box_.get_width_text()) / 2);

		text_.is_finished_ = false;
		text_.index_dialogue_ = 0;
		text_.text_ = "";
		text_.text_dialogue_ = "";
		text_.text_ = new_dialogue;

		if(!current_speaker_.empty())
		{
			text_.text_.insert(0, "\"");
			text_.text_.append("\"");
		}
	}
}

void Textbox::handle_events(const SDL_Event& e)
{
	switch(e.type)
	{
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym)
			{
				case SDLK_SPACE:
					//Prochain dialogue
					show_new_dialogue("And then, I would be I good guy because they are a lot of people that like somebody that used to be.", "Sayori");
					break;

				default:
					break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			if(e.button.button == SDL_BUTTON_LEFT)
			{
				//TODO : si sur la textbox OU sur rien du tout, alors passer au dialogue suivant
				show_new_dialogue("And then, I would be I good guy because they are a lot of people that like somebody that used to be.", "Sayori");
			}
			break;

		case SDL_KEYUP:
			break;

		default:
			break;
	}
}

void Textbox::draw(sdl::Renderer& renderer)
{
	if(!current_speaker_.empty())
	{
		namebox_.draw(renderer);
		if(!text_name_box_.text_.empty()) //TODO : cas à vérifier (pas utile car redondant ??)
		{
			text_name_box_.draw(renderer);
		}
	}
	textbox_.draw(renderer);
	text_.draw(renderer);
	if(text_.is_finished_)
	{
		triangle_.draw(renderer);
	}
}

void Textbox::update(Uint64 time_step) 
{
	text_.update(time_step);
	if(!text_name_box_.text_.empty())
	{
		text_name_box_.update(time_step);
	}
	if(text_.is_finished_)
	{
		if(time_step - triangle_.last_time_ > 500)
		{
			triangle_.show();
		}
		if(time_step - triangle_.last_time_ > 1000) 
		{
			triangle_.hide();
			triangle_.last_time_ = SDL_GetTicks64();
		}
	}
}
