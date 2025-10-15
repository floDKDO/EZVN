#include "image.h"

Image::Image(std::string path, SDL_Renderer* renderer)
	: alpha(255), angle(0), flip(SDL_FLIP_NONE), zorder(0)
{
	this->texture = IMG_LoadTexture(renderer, path.c_str());
	SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);

	int w, h;
	SDL_QueryTexture(this->texture, nullptr, nullptr, &w, &h);

	this->rect = {1280 / 2, 720 / 2, w, h};
}

Image::~Image()
{
	SDL_DestroyTexture(this->texture);
}

void Image::show(SDL_Renderer* renderer)
{
	this->alpha = 255;
	SDL_SetTextureAlphaMod(this->texture, this->alpha);
}

void Image::hide(SDL_Renderer* renderer)
{
	this->alpha = 0;
	SDL_SetTextureAlphaMod(this->texture, this->alpha);
}

void Image::flip_vertically()
{
	this->flip = SDL_FLIP_VERTICAL;
}

void Image::flip_horizontally()
{
	this->flip = SDL_FLIP_HORIZONTAL;
}

void Image::flip_normal()
{
	this->flip = SDL_FLIP_NONE;
}

void Image::resize(int w, int h)
{
	this->rect = {rect.x, rect.y, w, h};
}

void Image::position(int x, int y)
{
	this->rect = {x, y, rect.w, rect.h};
}