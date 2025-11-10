#include "image.h"

Image::Image(const std::string path, const int x, const int y, SDL_Renderer* renderer, const int zorder)
	: zorder(zorder), name(path), alpha(255), angle(0), flip(SDL_FLIP_NONE), r(255), g(255), b(255), frame_index(0), renderer(renderer), path(path)
{
	SDL_RWops* ops = SDL_RWFromFile(path.c_str(), "rb");

	if(IMG_isGIF(ops))
	{
		SDL_RWclose(ops);
		this->is_gif = true;
		this->gif = IMG_LoadAnimation(path.c_str());
		this->texture = SDL_CreateTextureFromSurface(renderer, gif->frames[this->frame_index]);
	}
	else
	{
		this->is_gif = false;
		this->gif = nullptr; 
		this->texture = IMG_LoadTexture(renderer, path.c_str());
	}

	SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);

	int w, h;
	SDL_QueryTexture(this->texture, nullptr, nullptr, &w, &h);

	this->position = {x, y, w, h};
}

Image::~Image()
{
	SDL_DestroyTexture(this->texture);
	if(this->is_gif)
		IMG_FreeAnimation(this->gif);
}

Image::Image(const Image& i)
{
	this->zorder = i.zorder;
	this->position = i.position;
	this->name = i.name;
	this->path = i.path;
	this->alpha = i.alpha;
	this->angle = i.angle;
	this->flip = i.flip;
	this->r = i.r;
	this->g = i.g;
	this->b = i.b;
	this->frame_index = i.frame_index;
	this->is_gif = i.is_gif;
	this->renderer = i.renderer;

	SDL_RWops* ops = SDL_RWFromFile(path.c_str(), "rb");

	if(IMG_isGIF(ops))
	{
		SDL_RWclose(ops);
		this->gif = IMG_LoadAnimation(path.c_str());
		this->texture = SDL_CreateTextureFromSurface(renderer, gif->frames[this->frame_index]);
	}
	else
	{
		this->gif = nullptr;
		this->texture = IMG_LoadTexture(renderer, path.c_str());
	}
}

Image& Image::operator=(const Image& i)
{
	if(this == &i)
	{
		return *this;
	}

	if(this->gif)
	{
		IMG_FreeAnimation(this->gif);
	}

	if(this->texture)
	{
		SDL_DestroyTexture(this->texture);
	}

	this->zorder = i.zorder;
	this->position = i.position;
	this->name = i.name;
	this->path = i.path;
	this->alpha = i.alpha;
	this->angle = i.angle;
	this->flip = i.flip;
	this->r = i.r;
	this->g = i.g;
	this->b = i.b;
	this->frame_index = i.frame_index;
	this->is_gif = i.is_gif;
	this->renderer = i.renderer;

	SDL_RWops* ops = SDL_RWFromFile(path.c_str(), "rb");

	if(IMG_isGIF(ops))
	{
		SDL_RWclose(ops);
		this->gif = IMG_LoadAnimation(path.c_str());
		this->texture = SDL_CreateTextureFromSurface(renderer, gif->frames[this->frame_index]);
	}
	else
	{
		this->gif = nullptr;
		this->texture = IMG_LoadTexture(renderer, path.c_str());
	}

	return *this;
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

void Image::resize(const int w, const int h)
{
	this->position = {position.x, position.y, w, h};
}

void Image::set_position(const int x, const int y)
{
	this->position = {x, y, position.w, position.h};
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

void Image::own_filter(const Uint8 r, const Uint8 g, const Uint8 b)
{
	this->r = r;
	this->g = g;
	this->b = b;
	SDL_SetTextureColorMod(this->texture, this->r, this->g, this->b);
}

void Image::draw(SDL_Renderer* renderer)
{
	SDL_RenderCopyEx(renderer, this->texture, nullptr, &(this->position), this->angle, nullptr, this->flip);
	if(is_gif)
	{
		SDL_DestroyTexture(this->texture);
		if(this->frame_index < this->gif->count - 1)
		{
			this->frame_index += 1;
		}
		else this->frame_index = 0;
		this->texture = SDL_CreateTextureFromSurface(renderer, this->gif->frames[this->frame_index]);
	}
}