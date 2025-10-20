#include "image.h"

Image::Image(const std::string path, const int x, const int y, SDL_Renderer* renderer, const int zorder)
	: name(path), alpha(255), angle(0), zorder(zorder), flip(SDL_FLIP_NONE), r(255), g(255), b(255), frame_index(0)
{
	SDL_RWops* ops = SDL_RWFromFile(path.c_str(), "rb");

	std::cout << "Constructeur " << this->name << std::endl;

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

//TODO : rule of 3/5 
//TODO : retirer les commentaires !!!
Image::~Image()
{
	/*
	std::cout << "Destructeur " << this->name << std::endl;
	SDL_DestroyTexture(this->texture);
	if(this->is_gif)
		IMG_FreeAnimation(this->gif);*/
}

/*
Image::Image(const Image& i)
{
	std::cout << "Copy constructor" << std::endl;
}

Image& Image::operator=(const Image& i)
{
	std::cout << "= operator" << std::endl;
	//return *(const_cast<Image*>(&i));
}
*/

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
	if(this->texture == nullptr)
	{
		std::cout << "texture nulle" << std::endl;
	}

	if(this->is_gif && this->gif == nullptr)
	{
		std::cout << "gif nul" << std::endl;
	}

	SDL_RenderCopyEx(renderer, this->texture, nullptr, &(this->position), this->angle, nullptr, this->flip);
	if(is_gif && this->gif != nullptr)
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