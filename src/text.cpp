#include "text.h"
#include <iostream>

Text::Text(const std::string text, const SDL_Color color, const std::string font_path, int font_size, const int x, const int y, SDL_Renderer* renderer)
	:text(text), color(color), font_size(font_size)
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
}

Text::~Text()
{
	TTF_CloseFont(this->font);
	SDL_DestroyTexture(this->texture);
	SDL_FreeSurface(this->surface);
}

//TODO : renommer en text_updated ??
void Text::edit_text(SDL_Renderer* renderer)
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
	this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
}

void Text::draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, this->texture, nullptr, &position);
}