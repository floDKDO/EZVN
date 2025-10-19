#include "text.h"

Text::Text(std::string text, SDL_Color color, std::string font_path, int x, int y, SDL_Renderer* renderer)
	:text(text), color(color)
{
	this->font = TTF_OpenFont(font_path.c_str(), 50); //TODO : taille ?

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
}

void Text::edit_text(std::string new_text, SDL_Renderer* renderer)
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