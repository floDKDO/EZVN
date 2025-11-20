#include "text.h"

#include <iostream>

Text::Text(const std::string text, const SDL_Color color, const std::string font_path, const int font_size, const int x, const int y, sdl::Renderer& renderer, Uint32 wrap_length)
	:text(text), color(color), font_size(font_size), font_style(0), renderer(renderer), wrap_length(wrap_length), text_dialogue(""), index_dialogue(0), previous_text(""), previous_font_style(0), font_path(font_path), is_finished(false), text_speed(15), last_time(0)
{
	this->font = std::make_unique<sdl::Font>(font_path, this->font_size); 

	int w, h;
	this->font->size_UTF8(this->text, &w, &h);
	this->position = {x, y, w, h};

	if(this->wrap_length != 0)
	{
		this->is_dialogue = true;
	}
	else
	{
		this->is_dialogue = false;
	}

	this->create_surface_texture();
}

void Text::create_surface_texture()
{
	if(this->text.empty() || (this->is_dialogue && this->text_dialogue.empty()))
	{
		this->surface = std::make_unique<sdl::Surface>(*this->font, " ", this->color, wrap_length);
	}
	else
	{
		if(this->is_dialogue)
		{
			this->surface = std::make_unique<sdl::Surface>(*(this->font), this->text_dialogue, this->color, wrap_length);
		}
		else
		{
			this->surface = std::make_unique<sdl::Surface>(*(this->font), this->text, this->color, wrap_length);
		}
	}
	this->position.w = this->surface->Get()->w;
	this->position.h = this->surface->Get()->h;

	this->texture = std::make_unique<sdl::Texture>(this->renderer, *(this->surface));
	this->texture->set_blend_mode(SDL_BLENDMODE_BLEND);
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

void Text::set_italic()
{
	this->font_style |= TTF_STYLE_ITALIC;
}

void Text::set_bold()
{
	this->font_style |= TTF_STYLE_BOLD;
}

void Text::set_underline()
{
	this->font_style |= TTF_STYLE_UNDERLINE;
}

void Text::set_strike_through()
{
	this->font_style |= TTF_STYLE_STRIKETHROUGH;
}

void Text::unset_italic()
{
	this->font_style &= ~TTF_STYLE_ITALIC;
}

void Text::unset_bold()
{
	this->font_style &= ~TTF_STYLE_BOLD;
}

void Text::unset_underline()
{
	this->font_style &= ~TTF_STYLE_UNDERLINE;
}

void Text::unset_strike_through()
{
	this->font_style &= ~TTF_STYLE_STRIKETHROUGH;
}

void Text::unset_all()
{
	this->font_style = TTF_STYLE_NORMAL;
}

int Text::get_width_one_char(char c)
{
	std::string s(1, c);
	int w;
	this->font->size_UTF8(s, &w, nullptr);
	return w;
}

int Text::get_width_text() //TODO : combiner les deux fonctions en une seule ??
{
	int w;
	this->font->size_UTF8(this->text, &w, nullptr);
	return w;
}

void Text::draw(sdl::Renderer& renderer)
{
	SDL_RenderCopy(renderer.Get(), this->texture->Get(), nullptr, &position);
}

//TODO : écrire le code spécifique aux dialogues dans la classe TextBox ??
//TODO : écrire le code spécifique aux inputfields dans la classe Inputfield ??
void Text::update(Uint64 time_step) 
{
	if(this->is_dialogue) 
	{
		if(time_step - this->last_time > this->text_speed && index_dialogue < text.length()) 
		{
			this->text_dialogue.push_back(this->text[index_dialogue]);
			index_dialogue += 1;
			last_time = SDL_GetTicks64();
		}

		if(index_dialogue == text.length() - 1)
		{
			this->is_finished = true;
		}
	}

	if(this->previous_font_style != this->font_style)
	{
		TTF_SetFontStyle(this->font->Get(), this->font_style);
		this->create_surface_texture();
		this->previous_font_style = this->font_style;
	}

	if(this->previous_text != this->text || (this->is_dialogue && this->previous_text != this->text_dialogue)) //the text has been modified (in a inputfield for example)
	{
		this->create_surface_texture();

		if(this->is_dialogue)
		{
			this->previous_text = this->text_dialogue;
		}
		else
		{
			this->previous_text = this->text;
		}
	}
}