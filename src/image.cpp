#include "image.h"

Image::Image(std::string path, SDL_Renderer* renderer)
	: alpha(255), angle(0), zorder(0), flip(SDL_FLIP_NONE), r(255), g(255), b(255)
{
	SDL_RWops* ops = SDL_RWFromFile(path.c_str(), "rb");

	if(IMG_isGIF(ops))
	{
		this->is_gif = true;
		this->frame_number = 0;
		this->gif = IMG_LoadAnimation(path.c_str());
		this->texture = SDL_CreateTextureFromSurface(renderer, gif->frames[this->frame_number]);
	}
	else
	{
		is_gif = false;
		this->texture = IMG_LoadTexture(renderer, path.c_str());
	}

	SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);

	int w, h;
	SDL_QueryTexture(this->texture, nullptr, nullptr, &w, &h);

	this->rect = {1280 / 4, 720 / 4, w, h};
}

Image::Image(std::string path, SDL_Renderer* renderer, int zorder)
	: alpha(255), angle(0), zorder(zorder), flip(SDL_FLIP_NONE), r(255), g(255), b(255)
{
	SDL_RWops* ops = SDL_RWFromFile(path.c_str(), "rb");

	if(IMG_isGIF(ops))
	{
		this->is_gif = true;
		this->frame_number = 0;
		this->gif = IMG_LoadAnimation(path.c_str());
		this->texture = SDL_CreateTextureFromSurface(renderer, gif->frames[this->frame_number]);
	}
	else
	{
		is_gif = false;
		this->texture = IMG_LoadTexture(renderer, path.c_str());
	}

	SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);

	int w, h;
	SDL_QueryTexture(this->texture, nullptr, nullptr, &w, &h);

	this->rect = {1280 / 4, 720 / 4, w, h};
}


Image::~Image()
{
	std::cout << "Destructeur !" << std::endl;
	SDL_DestroyTexture(this->texture);
}

void Image::show()
{
	this->alpha = 255;
	SDL_SetTextureAlphaMod(this->texture, this->alpha);
}

void Image::hide()
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

void Image::night_filter()
{
	this->r = 127;
	this->g = 127;
	this->b = 165;
	SDL_SetTextureColorMod(this->texture, this->r, this->g, this->b);
}

void Image::afternoon_filter()
{
	this->r = 210;
	this->g = 150;
	this->b = 130;
	SDL_SetTextureColorMod(this->texture, this->r, this->g, this->b);
}

void Image::own_filter(int r, int g, int b)
{
	this->r = r;
	this->g = g;
	this->b = b;
	SDL_SetTextureColorMod(this->texture, this->r, this->g, this->b);
}

void Image::draw(SDL_Renderer* renderer)
{
	SDL_RenderCopyEx(renderer, this->texture, nullptr, &(this->rect), this->angle, nullptr, this->flip);
	if(is_gif)
	{
		SDL_DestroyTexture(this->texture);
		if(this->frame_number < this->gif->count - 1)
		{
			this->frame_number += 1;
		}
		else this->frame_number = 0;
		this->texture = SDL_CreateTextureFromSurface(renderer, this->gif->frames[this->frame_number]);
	}
}