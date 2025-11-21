#include "image.h"
#include "RAII_SDL2/rwops.h"

#include <iostream>

Image::Image(const std::string path, const int x, const int y, sdl::Renderer& renderer, const int zorder)
	: zorder_(zorder), alpha_(255), angle_(0), flip_(SDL_FLIP_NONE), r_(255), g_(255), b_(255), frame_index_(0), renderer_(renderer), path_(path), last_time_(0)
{
	if(path_.find("img/characters/") != std::string::npos)
	{
		image_type_ = ImageType::CHARACTER;
	}
	else if(path_.find("img/gui/") != std::string::npos)
	{
		image_type_ = ImageType::GUI;
	}
	else if(path_.find("img/backgrounds/") != std::string::npos) 
	{
		image_type_ = ImageType::BACKGROUND;
	}
	else image_type_ = ImageType::NONE;

	if(image_type_ == ImageType::CHARACTER || image_type_ == ImageType::NONE)
	{
		sdl::RWops rwops(path, "rb"); 

		if(rwops.Get() != nullptr && IMG_isGIF(rwops.Get())) 
		{
			is_gif_ = true;
			gif_ = std::make_unique<sdl::Animation>(path);
			texture_ = std::make_unique<sdl::Texture>(renderer, gif_->Get()->frames[frame_index_]);
		}
		else
		{
			is_gif_ = false;
			texture_ = std::make_unique<sdl::Texture>(renderer, path);
		}
	}
	else
	{
		is_gif_ = false;
		texture_ = std::make_unique<sdl::Texture>(renderer, path);
	}

	texture_->set_blend_mode(SDL_BLENDMODE_BLEND);

	int w, h;
	texture_->query(nullptr, nullptr, &w, &h);

	position_ = {x, y, w, h};
}

void Image::show()
{
	alpha_ = 255;
	texture_->set_alpha_mod(alpha_);
}

void Image::hide()
{
	alpha_ = 0;
	texture_->set_alpha_mod(alpha_);
}

void Image::flip_vertically()
{
	flip_ = SDL_FLIP_VERTICAL;
}

void Image::flip_horizontally()
{
	flip_ = SDL_FLIP_HORIZONTAL;
}

void Image::flip_normal()
{
	flip_ = SDL_FLIP_NONE;
}

void Image::resize(const int w, const int h)
{
	position_ = {position_.x, position_.y, w, h};
}

void Image::set_position(const int x, const int y)
{
	position_ = {x, y, position_.w, position_.h};
}

void Image::night_filter()
{
	r_ = 127;
	g_ = 127;
	b_ = 165;
	texture_->set_color_mod(r_, g_, b_);
}

void Image::afternoon_filter()
{
	r_ = 210;
	g_ = 150;
	b_ = 130;
	texture_->set_color_mod(r_, g_, b_);
}

void Image::own_filter(const Uint8 r, const Uint8 g, const Uint8 b)
{
	r_ = r;
	g_ = g;
	b_ = b;
	texture_->set_color_mod(r_, g_, b_);
}

void Image::draw(sdl::Renderer& renderer)
{
	SDL_RenderCopyEx(renderer.Get(), texture_->Get(), nullptr, &(position_), angle_, nullptr, flip_);
	if(is_gif_)
	{
		if(frame_index_ < gif_->Get()->count - 1)
		{
			frame_index_ += 1;
		}
		else frame_index_ = 0;
		texture_ = std::make_unique<sdl::Texture>(renderer, gif_->Get()->frames[frame_index_]);
	}
}