#include "textbox.h"
#include "constants.h"

#include <iostream>

Textbox::Textbox(sdl::Renderer& renderer)
	: namebox_(constants::namebox_image_, 0, 0, renderer),
	textbox_(constants::textbox_image_, 0, 0, renderer),
	text_("", constants::textbox_text_color_, constants::textbox_font_, constants::textbox_text_size_, 0, 0, renderer, 10),
	text_name_box_("", constants::namebox_text_color_, constants::namebox_font_, constants::namebox_text_size_, 0, 0, renderer), //TODO : paramètres de position inutiles 
	triangle_(constants::textbox_end_dialogue_indicator_, 0, 0, renderer)
{
	textbox_.set_position((constants::window_width_ - textbox_.position_.w) / 2, constants::window_height_ - textbox_.position_.h + constants::textbox_y_delta_); 
	namebox_.set_position(textbox_.position_.x + constants::namebox_textbox_x_delta_, textbox_.position_.y - namebox_.position_.h);

	text_name_box_.position_.x = namebox_.position_.x + ((namebox_.position_.w - text_name_box_.get_width_text()) / 2);
	text_name_box_.position_.y = namebox_.position_.y + ((namebox_.position_.h - text_name_box_.get_height_text()) / 2);

	text_.position_.x = textbox_.position_.x + constants::textbox_text_x_delta_; 
	text_.position_.y = textbox_.position_.y + constants::textbox_text_y_delta_;

	text_.wrap_length_ = textbox_.position_.w - (constants::textbox_text_x_delta_ * 2); 
	std::cout << text_.wrap_length_ << std::endl;

	triangle_.position_.x = textbox_.position_.x + textbox_.position_.w + constants::textbox_end_dialogue_indicator_x_delta_; 
	triangle_.position_.y = textbox_.position_.y + constants::textbox_end_dialogue_indicator_y_delta_; 
}

void Textbox::show_new_dialogue(std::string new_dialogue, std::string speaker /*Character* speaker*/)
{
	if(text_.is_finished_)
	{
		//TODO : version à utiliser si Character* à la place de std::string
		/*if(speaker == nullptr) //Narrator
		{
			current_speaker_.clear();
		}
		else
		{
			current_speaker_ = speaker->name_;

			text_name_box_.text_ = current_speaker_;
			text_name_box_.position_.x = namebox_.position_.x + ((namebox_.position_.w - text_name_box_.get_width_text()) / 2);
		}*/

		if(speaker.empty()) //Narrator
		{
			current_speaker_.clear();
		}
		else
		{
			current_speaker_ = speaker;

			text_name_box_.text_ = current_speaker_;
			text_name_box_.position_.x = namebox_.position_.x + ((namebox_.position_.w - text_name_box_.get_width_text()) / 2);
		}

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
		text_name_box_.draw(renderer);
	}
	textbox_.draw(renderer);
	text_.draw(renderer);
	if(text_.is_finished_)
	{
		triangle_.draw(renderer);
	}
}

void Textbox::update() 
{
	text_.update();
	if(!text_name_box_.text_.empty())
	{
		text_name_box_.update();
	}

	Uint64 now = SDL_GetTicks64();
	if(text_.is_finished_)
	{
		if(now - triangle_.last_time_ > 500)
		{
			triangle_.show();
		}
		if(now - triangle_.last_time_ > 1000) 
		{
			triangle_.hide();
			triangle_.last_time_ = SDL_GetTicks64();
		}
	}
}
