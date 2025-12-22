#include "image.h"
#include "constants.h"
#include "RAII_SDL2/rwops.h"

#include <iostream>

Image::Image(const std::string_view path, const int x, const int y, sdl::Renderer& renderer, const int zorder)
	: Drawable(renderer), zorder_(zorder), animated_image_(create_animation(path)), path_(path), frame_index_(0)
{
	init_image(path, x, y, renderer);
}

std::optional<sdl::Animation> Image::create_animation(const std::string_view path)
{
	sdl::RWops rwops(path, "rb");

	if(rwops.is_animation())
	{
		return sdl::Animation(path);
	}
	else
	{
		return std::nullopt;
	}
}

void Image::init_image(const std::string_view new_path, const int x, const int y, sdl::Renderer& renderer)
{
	path_ = new_path;
	position_.x = x;
	position_.y = y;

	if(path_.find("img/characters/") != std::string_view::npos)
	{
		image_type_ = ImageType::CHARACTER;
	}
	else if(path_.find("img/gui/") != std::string_view::npos)
	{
		image_type_ = ImageType::GUI;
	}
	else if(path_.find("img/backgrounds/") != std::string_view::npos)
	{
		image_type_ = ImageType::BACKGROUND;
	}
	else 
	{
		image_type_ = ImageType::NONE;
	}

	if(animated_image_.has_value())
	{
		texture_ = std::make_unique<sdl::Texture>(renderer, animated_image_->fetch()->frames[frame_index_]);
	}
	else
	{
		texture_ = std::make_unique<sdl::Texture>(renderer, new_path);
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
	if(animated_image_.has_value())
	{
		if(frame_index_ < animated_image_->fetch()->count - 1)
		{
			frame_index_ += 1;
		}
		else 
		{
			frame_index_ = 0;
		}
		texture_ = std::make_unique<sdl::Texture>(renderer, animated_image_->fetch()->frames[frame_index_]);
	}
}