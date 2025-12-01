#include "GUI/inputfield.h"

#include <iostream>

Inputfield::Inputfield(const std::string path, const SDL_Color color_normal, unsigned int character_limit, const int x, const int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: Ui(renderer), text_("", color_normal, "fonts/Aller_Rg.ttf", 50, x+7, y, renderer),
	  normal_(path, x, y, renderer), color_normal_(color_normal), 
	  character_limit_(character_limit),
	  text_caret_("|", color_normal, "fonts/Aller_Rg.ttf", 50, x, y, renderer), 
	  text_placeholder_("...", color_normal, "fonts/Aller_Rg.ttf", 50, x, y, renderer), 
	  is_editing_(false), is_writing_(false), index_caret_(0), offset_caret_(0)
{
	callback_function_ = callback_function;
	SDL_SetTextInputRect(&(normal_.position_));

	pointer_on_ui_when_pointer_up_ = true;
}

void Inputfield::on_pointer_up_hook_end()
{
	is_editing_ = !is_editing_;
}

void Inputfield::on_left_pressed()
{
	if(is_editing_)
	{
		if(index_caret_ > 0)
		{
			int current_char_width = text_.get_width_one_char(text_.text_[index_caret_ - 1]);
			offset_caret_ += -current_char_width;
			index_caret_ -= 1;
		}
	}
	else
	{
		Ui::on_left_pressed();
	}
}

void Inputfield::on_right_pressed()
{
	if(is_editing_)
	{
		if(index_caret_ < character_limit_ && index_caret_ < text_.text_.length())
		{
			int current_char_width = text_.get_width_one_char(text_.text_[index_caret_]);
			offset_caret_ += current_char_width;
			index_caret_ += 1;
		}
	}
	else
	{
		Ui::on_right_pressed();
	}
}

void Inputfield::on_enter_pressed_hook_end()
{
	is_editing_ = !is_editing_;
}

void Inputfield::on_backspace_pressed()
{
	if(is_editing_ && !text_.text_.empty())
	{
		if(index_caret_ > 0)
		{
			text_.text_.erase(index_caret_ - 1, 1);
			index_caret_ -= 1;
		}
	}
}

void Inputfield::on_delete_pressed()
{
	if(is_editing_ && !text_.text_.empty())
	{
		if(index_caret_ < text_.text_.length())
		{
			int current_char_width = text_.get_width_one_char(text_.text_[index_caret_]);
			offset_caret_ += current_char_width;
			text_.text_.erase(index_caret_, 1);
		}
	}
}

void Inputfield::on_input_pressed_hook_end(const SDL_Event& e)
{
	if(is_editing_)
	{
		is_writing_ = true;
	}

	if(e.key.keysym.sym == SDLK_BACKSPACE)
	{
		on_backspace_pressed();
	}
	else if(e.key.keysym.sym == SDLK_DELETE)
	{
		on_delete_pressed();
	}
}

void Inputfield::on_input_released_hook_end(const SDL_Event& e)
{
	(void)e;
	is_writing_ = false;
}

void Inputfield::handle_events_hook_end(const SDL_Event& e)
{
	if(e.type == SDL_TEXTINPUT)
	{
		on_typing(e);
	}
}

void Inputfield::draw(sdl::Renderer& renderer)
{
	normal_.draw(renderer);
	text_.draw(renderer);

	if(text_.text_.empty()) 
	{
		if(!is_editing_)
		{
			text_placeholder_.draw(renderer);
		}
		text_caret_.position_.x = get_rect().x;
	}
	else text_caret_.position_.x = get_rect().x + text_.position_.w + offset_caret_;

	if(is_editing_)
	{
		text_caret_.draw(renderer);
	}
}

void Inputfield::update()
{
	text_.update();

	Uint64 now = SDL_GetTicks64();
	if(now - last_time_ > 500)
	{
		text_caret_.show();
	}
	if(now - last_time_ > 1000 && !is_writing_) //do not hide the caret when writing/deleting
	{
		text_caret_.hide();
		last_time_ = SDL_GetTicks64();
	}
}

void Inputfield::on_typing(const SDL_Event& e)
{
	if(is_editing_ && text_.text_.length() < character_limit_)
	{
		text_.text_.insert(text_.text_.begin() + index_caret_, e.text.text[0]);
		index_caret_ += 1;
	}
}

void Inputfield::set_character_limit(unsigned int character_limit)
{
	character_limit_ = character_limit;
}

SDL_Rect Inputfield::get_rect() const 
{
	return normal_.position_;
}