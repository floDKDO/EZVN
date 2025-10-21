#include "text.h"

Text::Text(const std::string text, const SDL_Color color, const std::string font_path, const int font_size, const int x, const int y, SDL_Renderer* renderer)
	:text(text), color(color), font_size(font_size), renderer(renderer)
{
	this->font = TTF_OpenFont(font_path.c_str(), this->font_size); 

	int w, h;
	TTF_SizeUTF8(this->font, this->text.c_str(), &w, &h);
	this->position = {x, y, w, h};

	if(this->text.empty())
	{
		this->surface = TTF_RenderUTF8_Blended(this->font, " ", this->color);
	}
	else
	{
		this->surface = TTF_RenderUTF8_Blended(this->font, this->text.c_str(), this->color);
	}
	this->position.w = this->surface->w;
	this->position.h = this->surface->h;
	this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
	SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
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

void Text::update(Uint64& time_step)
{
	static std::string previous_text;
	if(previous_text != this->text) //the text has been modified (in a inputfield for example)
	{
		SDL_DestroyTexture(this->texture);
		SDL_FreeSurface(this->surface);

		if(this->text.empty())
		{
			this->surface = TTF_RenderUTF8_Blended(this->font, " ", this->color);
		}
		else
		{
			this->surface = TTF_RenderUTF8_Blended(this->font, this->text.c_str(), this->color);
		}
		this->position.w = this->surface->w;
		this->position.h = this->surface->h;
		this->texture = SDL_CreateTextureFromSurface(this->renderer, this->surface);
		SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
	}
	previous_text = this->text;
}