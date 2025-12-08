#include "image.h"
#include "RAII_SDL2/rwops.h"
#include "constants.h"

#include <iostream>

Image::Image(const std::string path, const int x, const int y, sdl::Renderer& renderer, const int zorder)
	: zorder_(zorder), path_(path), frame_index_(0), renderer_(renderer)
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

void Image::draw(sdl::Renderer& renderer)
{
	//std::cout << "HEHEHEHEHEHE " << path_ << std::endl;
	renderer.copy(*texture_, nullptr, &(position_), angle_, nullptr, flip_);
	//SDL_RenderCopyEx(renderer.fetch(), texture_->fetch(), nullptr, &(position_), angle_, nullptr, flip_);
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