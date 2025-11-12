#include "text.h"

Text::Text(const std::string text, const SDL_Color color, const std::string font_path, const int font_size, const int x, const int y, SDL_Renderer* renderer, Uint32 wrap_length)
	:text(text), color(color), font_size(font_size), renderer(renderer), wrap_length(wrap_length), text_dialogue(""), index_dialogue(0), previous_text(""), font_path(font_path)
{
	this->font = std::make_unique<sdl::Font>(font_path, this->font_size); 

	int w, h;
	this->font->size_UTF8(this->text, &w, &h);
	this->position = {x, y, w, h};

	if(this->text.empty())
	{
		this->surface = std::make_unique<sdl::Surface>(this->font->Get(), " ", this->color, wrap_length);
	}
	else
	{
		this->surface = std::make_unique<sdl::Surface>(this->font->Get(), this->text, this->color, wrap_length);
	}

	this->position.w = this->surface->Get()->w;
	this->position.h = this->surface->Get()->h;

	this->texture = std::make_unique<sdl::Texture>(renderer, this->surface->Get());
	this->texture->set_blend_mode(SDL_BLENDMODE_BLEND);

	if(this->wrap_length != 0)
	{
		this->is_dialogue = true;
	}
	else
	{
		this->is_dialogue = false;
	}
}

void Text::show()
{
	this->color.a = 255;
	this->texture->set_alpha_mod(this->color.a);
}

void Text::hide()
{
	this->color.a = 0;
	this->texture->set_alpha_mod(this->color.a);
}

void Text::change_color(SDL_Color color)
{
	this->color = color;
	this->texture->set_color_mod(this->color.r, this->color.g, this->color.b);
}

int Text::get_width_one_char(char c)
{
	std::string s(1, c);
	int w;
	this->font->size_UTF8(s, &w, nullptr);
	return w;
}

void Text::draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, this->texture->Get(), nullptr, &position);
}

void Text::update(Uint64& time_step)
{
	(void)time_step;

	if(this->is_dialogue) 
	{
		if(SDL_GetTicks64() - time_step > 10 && index_dialogue < text.length()) //10 => vitesse de défilement 
		{
			this->text_dialogue.push_back(this->text[index_dialogue]);
			index_dialogue += 1;
			time_step = SDL_GetTicks64();
		}
	}

	if(this->previous_text != this->text || (this->is_dialogue && this->previous_text != this->text_dialogue)) //the text has been modified (in a inputfield for example)
	{
		if(this->text.empty() || (this->is_dialogue && this->text_dialogue.empty()))
		{
			this->surface = std::make_unique<sdl::Surface>(this->font->Get(), " ", this->color, wrap_length);
		}
		else
		{
			if(this->is_dialogue)
			{
				this->surface = std::make_unique<sdl::Surface>(this->font->Get(), this->text_dialogue, this->color, wrap_length);
			}
			else
			{
				this->surface = std::make_unique<sdl::Surface>(this->font->Get(), this->text, this->color, wrap_length);
			}
		}
		this->position.w = this->surface->Get()->w;
		this->position.h = this->surface->Get()->h;
		
		this->texture = std::make_unique<sdl::Texture>(renderer, this->surface->Get());
		this->texture->set_blend_mode(SDL_BLENDMODE_BLEND);
	}
	if(this->is_dialogue)
	{
		this->previous_text = this->text_dialogue;
	}
	else
	{
		this->previous_text = this->text;
	}
}