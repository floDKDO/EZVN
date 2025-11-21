#include "image.h"
#include "RAII_SDL2/rwops.h"

#include <iostream>

Image::Image(const std::string path, const int x, const int y, sdl::Renderer& renderer, const int zorder)
	: zorder(zorder), alpha(255), angle(0), flip(SDL_FLIP_NONE), r(255), g(255), b(255), frame_index(0), renderer(renderer), path(path), last_time(0)
{
	if(this->path.find("img/characters/") != std::string::npos)
	{
		this->image_type = ImageType::CHARACTER;
	}
	else if(this->path.find("img/gui/") != std::string::npos)
	{
		this->image_type = ImageType::GUI;
	}
	else if(this->path.find("img/backgrounds/") != std::string::npos) 
	{
		this->image_type = ImageType::BACKGROUND;
	}
	else this->image_type = ImageType::NONE;

	if(this->image_type == ImageType::CHARACTER || this->image_type == ImageType::NONE)
	{
		sdl::RWops rwops(path, "rb"); 

		if(rwops.Get() != nullptr && IMG_isGIF(rwops.Get())) 
		{
			this->is_gif = true;
			this->gif = std::make_unique<sdl::Animation>(path);
			this->texture = std::make_unique<sdl::Texture>(renderer, gif->Get()->frames[this->frame_index]);
		}
		else
		{
			this->is_gif = false;
			this->texture = std::make_unique<sdl::Texture>(renderer, path);
		}
	}
	else
	{
		this->is_gif = false;
		this->texture = std::make_unique<sdl::Texture>(renderer, path);
	}

	this->texture->set_blend_mode(SDL_BLENDMODE_BLEND);

	int w, h;
	this->texture->query(nullptr, nullptr, &w, &h);

	this->position = {x, y, w, h};
}

void Image::show()
{
	this->alpha = 255;
	this->texture->set_alpha_mod(this->alpha);
}

void Image::hide()
{
	this->alpha = 0;
	this->texture->set_alpha_mod(this->alpha);
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
	this->texture->set_color_mod(this->r, this->g, this->b);
}

void Image::afternoon_filter()
{
	this->r = 210;
	this->g = 150;
	this->b = 130;
	this->texture->set_color_mod(this->r, this->g, this->b);
}

void Image::own_filter(const Uint8 r, const Uint8 g, const Uint8 b)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->texture->set_color_mod(this->r, this->g, this->b);
}

void Image::draw(sdl::Renderer& renderer)
{
	SDL_RenderCopyEx(renderer.Get(), this->texture->Get(), nullptr, &(this->position), this->angle, nullptr, this->flip);
	if(is_gif)
	{
		if(this->frame_index < this->gif->Get()->count - 1)
		{
			this->frame_index += 1;
		}
		else this->frame_index = 0;
		this->texture = std::make_unique<sdl::Texture>(renderer, gif->Get()->frames[this->frame_index]);
	}
}