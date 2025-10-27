#include "text.h"

#include <iostream>

Text::Text(const std::string text, const SDL_Color color, const std::string font_path, const int font_size, const int x, const int y, SDL_Renderer* renderer, Uint32 wrap_length)
	:text(text), color(color), font_size(font_size), renderer(renderer), wrap_length(wrap_length)
{
	this->font = TTF_OpenFont(font_path.c_str(), this->font_size); 

	int w, h;
	TTF_SizeUTF8(this->font, this->text.c_str(), &w, &h);
	this->position = {x, y, w, h};

	if(this->text.empty())
	{
		this->surface = TTF_RenderUTF8_Blended_Wrapped(this->font, " ", this->color, wrap_length);
	}
	else
	{
		this->surface = TTF_RenderUTF8_Blended_Wrapped(this->font, this->text.c_str(), this->color, wrap_length);
	}

	this->position.w = this->surface->w;
	this->position.h = this->surface->h;
	this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
	SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);

	this->text_dialogue = "";
	this->index_dialogue = 0;
}

Text::~Text()
{
	TTF_CloseFont(this->font);
	SDL_DestroyTexture(this->texture);
	SDL_FreeSurface(this->surface);
}

void Text::show()
{
	this->color.a = 255;
	SDL_SetTextureAlphaMod(this->texture, this->color.a);
}

void Text::hide()
{
	this->color.a = 0;
	SDL_SetTextureAlphaMod(this->texture, this->color.a);
}

void Text::change_color(SDL_Color color)
{
	this->color = color;
	SDL_SetTextureColorMod(this->texture, this->color.r, this->color.g, this->color.b);
}

void Text::draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, this->texture, nullptr, &position);
}

//TODO : améliorer le code...
void Text::update(Uint64& time_step)
{
	(void)time_step;
	if(this->wrap_length != 0) //is dialogue
	{
		std::cout << "DIALOGUE!\n";
		if(SDL_GetTicks64() - time_step > 10 && index_dialogue < text.length()) //10 => vitesse de défilement 
		{
			this->text_dialogue.push_back(this->text[index_dialogue]);
			std::cout << text_dialogue << std::endl;
			index_dialogue += 1;
			time_step = SDL_GetTicks64();
		}
	}
	static std::string previous_text;
	if(previous_text != this->text || previous_text != this->text_dialogue) //the text has been modified (in a inputfield for example)
	{
		std::cout << "TEXT EDITED\n";
		SDL_DestroyTexture(this->texture);
		SDL_FreeSurface(this->surface);

		if(this->text.empty() || this->text_dialogue.empty())
		{
			this->surface = TTF_RenderUTF8_Blended_Wrapped(this->font, " ", this->color, wrap_length);
		}
		else
		{
			if(this->wrap_length != 0)
			{
				this->surface = TTF_RenderUTF8_Blended_Wrapped(this->font, this->text_dialogue.c_str(), this->color, wrap_length);
			}
			else
			{
				this->surface = TTF_RenderUTF8_Blended_Wrapped(this->font, this->text.c_str(), this->color, wrap_length);
			}
		}
		this->position.w = this->surface->w;
		this->position.h = this->surface->h;
		this->texture = SDL_CreateTextureFromSurface(this->renderer, this->surface);
		SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
	}
	previous_text = this->text;
}