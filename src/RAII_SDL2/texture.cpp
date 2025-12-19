#include "RAII_SDL2/texture.h"
#include "RAII_SDL2/renderer.h"

#include <SDL2/SDL_image.h>
#include <iostream>

namespace sdl
{

Texture::Texture(Renderer& renderer, Surface& surface) //SDL_CreateTextureFromSurface()
{
	if((texture_ = SDL_CreateTextureFromSurface(renderer.fetch(), surface.fetch())) == nullptr)
	{
		SDL_Log("(SDL_CreateTextureFromSurface) %s\n", SDL_GetError());
	}
}

Texture::Texture(Renderer& renderer, SDL_Surface* surface) //SDL_CreateTextureFromSurface() with surface a frame from a IMG_Animation*
{
	if((texture_ = SDL_CreateTextureFromSurface(renderer.fetch(), surface)) == nullptr)
	{
		SDL_Log("(SDL_CreateTextureFromSurface) %s\n", SDL_GetError());
	}
}

Texture::Texture(Renderer& renderer, const std::string_view file) //IMG_LoadTexture()
{
	texture_ = IMG_LoadTexture(renderer.fetch(), file.data());
	if(texture_ == nullptr) //TODO: gérer le cas pour les backgrounds
	{
		std::cerr << "The image at the path " << file << " was not found...\n";
		if((texture_ = IMG_LoadTexture(renderer.fetch(), "img/characters/image_not_found.png")) == nullptr)
		{
			SDL_Log("(IMG_LoadTexture) %s\n", IMG_GetError());
		}
	}
}

Texture::Texture(Texture&& texture)
	: texture_(texture.texture_)
{
	texture.texture_ = nullptr;
}

Texture& Texture::operator=(Texture&& texture)
{
	if(this == &texture)
	{
		return *this;
	}

	if(texture_ != nullptr)
	{
		SDL_DestroyTexture(texture_);
	}

	texture_ = texture.texture_;
	texture.texture_ = nullptr;
	return *this;
}

Texture::~Texture() //SDL_DestroyTexture
{
	if(texture_ != nullptr)
	{
		SDL_DestroyTexture(texture_);
	}
}

SDL_Texture* Texture::fetch() const
{
	return texture_;
}

void Texture::set_alpha_mod(const Uint8 alpha) const
{
	if(SDL_SetTextureAlphaMod(texture_, alpha) < 0)
	{
		SDL_Log("(SDL_SetTextureAlphaMod) %s\n", SDL_GetError());
	}
}

void Texture::set_color_mod(const Uint8 r, const Uint8 g, const Uint8 b) const
{
	if(SDL_SetTextureColorMod(texture_, r, g, b) < 0)
	{
		SDL_Log("(SDL_SetTextureColorMod) %s\n", SDL_GetError());
	}
}

void Texture::set_blend_mode(const SDL_BlendMode blend_mode) const
{
	if(SDL_SetTextureBlendMode(texture_, blend_mode) < 0)
	{
		SDL_Log("(SDL_SetTextureBlendMod) %s\n", SDL_GetError());
	}
}

void Texture::query(Uint32* format, int* access, int* w, int* h) const
{
	if(SDL_QueryTexture(texture_, format, access, w, h) < 0)
	{
		SDL_Log("(SDL_QueryTexture) %s\n", SDL_GetError());
	}
}

}
