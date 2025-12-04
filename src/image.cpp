#include "image.h"
#include "RAII_SDL2/rwops.h"
#include "constants.h"

#include <iostream>

Image::Image(const std::string path, const int x, const int y, sdl::Renderer& renderer, const int zorder)
	: zorder_(zorder), last_time_(0), path_(path), alpha_(255), angle_(0), flip_(SDL_FLIP_NONE), r_(255), g_(255), b_(255), frame_index_(0), renderer_(renderer)
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

		if(rwops.fetch() != nullptr && (IMG_isGIF(rwops.fetch()) || IMG_isWEBP(rwops.fetch())))
		{
			is_animated_ = true;
			animated_image_ = std::make_unique<sdl::Animation>(path);
			texture_ = std::make_unique<sdl::Texture>(renderer, animated_image_->fetch()->frames[frame_index_]);
		}
		else
		{
			is_animated_ = false;
			texture_ = std::make_unique<sdl::Texture>(renderer, path);
		}
	}
	else
	{
		is_animated_ = false;
		texture_ = std::make_unique<sdl::Texture>(renderer, path);
	}

	texture_->set_blend_mode(SDL_BLENDMODE_BLEND);

	int w, h;
	texture_->query(nullptr, nullptr, &w, &h);

	position_ = {x, y, w, h};
	initial_rect_ = position_;
}

void Image::show()
{
	//std::cout << "SHOW ***********************************************************\n";
	alpha_ = 255;
	texture_->set_alpha_mod(alpha_);
}

void Image::hide()
{
	//std::cout << "HIDE ***********************************************************\n";
	alpha_ = 0;
	texture_->set_alpha_mod(alpha_);
}

void Image::rotate(const double angle)
{
	angle_ = angle;
}

void Image::flip_vertically()
{
	//std::cout << "FLIP V ***********************************************************\n";
	flip_ = SDL_FLIP_VERTICAL;
}

void Image::flip_horizontally()
{
	//std::cout << "FLIP H ***********************************************************\n";
	flip_ = SDL_FLIP_HORIZONTAL;
}

void Image::flip_normal()
{
	//std::cout << "FLIP N ***********************************************************\n";
	flip_ = SDL_FLIP_NONE;
}

int Image::get_xcenter() const
{
	//std::cout << "GET XCENTER ***********************************************************\n";
	return position_.w / 2;
}

int Image::get_ycenter() const
{
	//std::cout << "GET YCENTER ***********************************************************\n";
	return position_.h / 2;
}

void Image::zoom(const float zoom)
{
	//std::cout << "ZOOM ***********************************************************\n";

	float position_w = float(position_.w);
	float position_h = float(position_.h);

	position_w *= zoom;
	position_h *= zoom;

	position_.w = int(position_w);
	position_.h = int(position_h);
}

void Image::resize(const int w, const int h)
{
	//std::cout << "RESIZE ***********************************************************\n";
	position_ = {position_.x, position_.y, w, h};
}

void Image::set_position(const int x, const int y)
{
	//std::cout << "SET POS ***********************************************************\n";
	position_ = {x, y, position_.w, position_.h};
}

void Image::set_position_xcenter(const int x, const int y)
{
	//std::cout << "SET POS XCENTER ***********************************************************\n";
	position_ = {x - std::abs(get_xcenter()), y, position_.w, position_.h};
}

void Image::set_center()
{
	position_ = {constants::window_width_ / 2 - std::abs(get_xcenter()), constants::window_height_ / 2 - std::abs(get_ycenter()), position_.w, position_.h};
}

void Image::night_filter()
{
	//std::cout << "N FILTER ***********************************************************\n";
	r_ = 127;
	g_ = 127;
	b_ = 165;
	texture_->set_color_mod(r_, g_, b_);
}

void Image::afternoon_filter()
{
	//std::cout << "A FILTER ***********************************************************\n";
	r_ = 210;
	g_ = 150;
	b_ = 130;
	texture_->set_color_mod(r_, g_, b_);
}

void Image::own_filter(const Uint8 r, const Uint8 g, const Uint8 b)
{
	//std::cout << "O FILTER ***********************************************************\n";
	r_ = r;
	g_ = g;
	b_ = b;
	texture_->set_color_mod(r_, g_, b_);
}

void Image::draw(sdl::Renderer& renderer)
{
	SDL_RenderCopyEx(renderer.fetch(), texture_->fetch(), nullptr, &(position_), angle_, nullptr, flip_);
	if(is_animated_)
	{
		if(frame_index_ < animated_image_->fetch()->count - 1)
		{
			frame_index_ += 1;
		}
		else frame_index_ = 0;
		texture_ = std::make_unique<sdl::Texture>(renderer, animated_image_->fetch()->frames[frame_index_]);
	}
}