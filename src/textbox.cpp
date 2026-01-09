#include "textbox.h"
#include "constants.h"

#include <algorithm>
#include <iostream>

const Uint64 Textbox::minimum_time_ = 3000; //3s
const Uint64 Textbox::maximum_time_ = 10000; //10s
const Uint64 Textbox::base_delay_ = 1750; //1.75s

Textbox::Textbox(sdl::Renderer& renderer)
	: text_("", constants::textbox_text_color_, constants::textbox_font_, constants::textbox_text_size_, 0, 0, renderer, true, 10),
	textbox_(constants::textbox_image_, 0, 0, renderer), current_speaker_(""), 
	namebox_(constants::namebox_image_, 0, 0, renderer),
	text_name_box_("", constants::namebox_text_color_, constants::namebox_font_, constants::namebox_text_size_, 0, 0, renderer), //TODO : paramètres de position inutiles 
	triangle_(constants::textbox_end_dialogue_indicator_, 0, 0, renderer), is_first_dialogue_(true)
{
	textbox_.set_position((constants::window_width_ - textbox_.position_.w) / 2, constants::window_height_ - textbox_.position_.h + constants::textbox_y_delta_); 
	namebox_.set_position(textbox_.position_.x + constants::namebox_textbox_x_delta_, textbox_.position_.y - namebox_.position_.h);

	text_name_box_.position_.x = namebox_.position_.x + ((namebox_.position_.w - text_name_box_.get_width_text()) / 2);
	text_name_box_.position_.y = namebox_.position_.y + ((namebox_.position_.h - text_name_box_.get_height_text()) / 2);

	text_.position_.x = textbox_.position_.x + constants::textbox_text_x_delta_; 
	text_.position_.y = textbox_.position_.y + constants::textbox_text_y_delta_;

	text_.wrap_length_ = textbox_.position_.w - (constants::textbox_text_x_delta_ * 2); 

	triangle_.position_.x = textbox_.position_.x + textbox_.position_.w + constants::textbox_end_dialogue_indicator_x_delta_; 
	triangle_.position_.y = textbox_.position_.y + constants::textbox_end_dialogue_indicator_y_delta_; 
}

void Textbox::change_textbox(std::string_view new_textbox_path, sdl::Renderer& renderer)
{
	if(new_textbox_path.empty())
	{
		new_textbox_path = constants::textbox_image_;
	}
	textbox_.init_image(new_textbox_path, textbox_.position_.x, textbox_.position_.y, renderer);
}

void Textbox::change_namebox(std::string_view new_namebox_path, sdl::Renderer& renderer)
{
	if(new_namebox_path.empty())
	{
		new_namebox_path = constants::namebox_image_;
	}
	namebox_.init_image(new_namebox_path, namebox_.position_.x, namebox_.position_.y, renderer);
}

void Textbox::change_namebox_text_color(Color new_namebox_text_color)
{
	text_name_box_.change_color(new_namebox_text_color);
}

void Textbox::show_new_dialogue(const std::string_view new_dialogue, std::string speaker, bool in_skip_mode, bool wait_for_end_of_dialogue)
{
	if(is_first_dialogue_ || (text_.is_finished_ && wait_for_end_of_dialogue) || !wait_for_end_of_dialogue)
	{
		current_speaker_ = speaker;
		text_name_box_.text_ = current_speaker_;
		text_name_box_.position_.x = namebox_.position_.x + ((namebox_.position_.w - text_name_box_.get_width_text()) / 2);
		
		text_.is_finished_ = false;
		text_.index_dialogue_ = 0;
		text_.text_ = "";
		text_.text_dialogue_ = "";
		text_.text_ = new_dialogue;
		text_.is_animated_ = !in_skip_mode;

		if(!current_speaker_.empty())
		{
			text_.text_.insert(0, "\"");
			text_.text_.append("\"");
		}

		is_first_dialogue_ = false;
	}
}

Uint64 Textbox::get_text_delay()
{
	return std::clamp(static_cast<Uint64>(Textbox::base_delay_ + (float(text_.text_.length()) / (Text::global_text_divisor_ / 2) * 1000)), Textbox::minimum_time_, Textbox::maximum_time_);
}

void Textbox::handle_events(const SDL_Event& e)
{
	/*switch(e.type)
	{
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym)
			{
				case SDLK_SPACE:
					//Prochain dialogue
					//show_new_dialogue("And then, I would be I good guy because they are a lot of people that like somebody that used to be.", "Sayori");
					break;

				default:
					break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			if(e.button.button == SDL_BUTTON_LEFT)
			{
				//show_new_dialogue("And then, I would be I good guy because they are a lot of people that like somebody that used to be.", "Sayori");
			}
			break;

		case SDL_KEYUP:
			break;

		default:
			break;
	}*/
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
