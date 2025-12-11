#include "image.h"
#include "constants.h"
#include "RAII_SDL2/rwops.h"

#include <variant>
#include <iostream>

Image::Image(const std::string_view path, const int x, const int y, sdl::Renderer& renderer, const int zorder)
	: Drawable(renderer), zorder_(zorder), animated_image_(create_animation(path)), path_(path), frame_index_(0)
{
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
		texture_ = std::make_unique<sdl::Texture>(renderer, path);
	}

	////if(image_type_ == ImageType::CHARACTER || image_type_ == ImageType::NONE)
	//{
	//	//TODO : mettre ce test dans la classe Animation ?? Si oui, utiliser IMG_LoadGIFAnimation_RW et IMG_LoadWEBPAnimation_RW
	//	//sdl::RWops rwops(path, "rb"); 

	//	//if(rwops.is_gif() || rwops.is_webp())
	//	{
	//		animated_image_ = std::make_unique<sdl::Animation>(path);
	//		texture_ = std::make_unique<sdl::Texture>(renderer, animated_image_->fetch()->frames[frame_index_]);
	//	}
	//	/*else
	//	{
	//		texture_ = std::make_unique<sdl::Texture>(renderer, path);
	//	}*/
	//}
	///*else
	//{
	//	texture_ = std::make_unique<sdl::Texture>(renderer, path);
	//}*/

	texture_->set_blend_mode(SDL_BLENDMODE_BLEND);

	int w, h;
	texture_->query(nullptr, nullptr, &w, &h);

	position_ = {x, y, w, h};
	initial_rect_ = position_;
}

std::optional<sdl::Animation> Image::create_animation(const std::string_view path)
{
	sdl::RWops rwops(path, "rb");

	//TODO : redondant avec la vérification dans la classe Animation ??
	if(rwops.is_gif() || rwops.is_webp())
	{
		return sdl::Animation(path);
	}
	else
	{
		return std::nullopt;
	}
}

//TODO : trop de répétitions avec le constructeur
void Image::change_image(const std::string_view new_path, const int x, const int y, sdl::Renderer& renderer)
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

	////if(image_type_ == ImageType::CHARACTER || image_type_ == ImageType::NONE)
	//{
	//	//sdl::RWops rwops(path_, "rb");

	//	//if(rwops.is_gif() || rwops.is_webp())
	//	{
	//		animated_image_ = std::make_unique<sdl::Animation>(path_);
	//		texture_ = std::make_unique<sdl::Texture>(renderer, animated_image_->fetch()->frames[frame_index_]);
	//	}
	//	/*else
	//	{
	//		texture_ = std::make_unique<sdl::Texture>(renderer, path_);
	//	}*/
	//}
	///*else
	//{
	//	texture_ = std::make_unique<sdl::Texture>(renderer, path_);
	//}*/

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
		else frame_index_ = 0;
		//TODO
		texture_ = std::make_unique<sdl::Texture>(renderer, animated_image_->fetch()->frames[frame_index_]);
	}
}