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

Texture::Texture(Renderer& renderer, const std::string file) //IMG_LoadTexture()
{
	texture_ = IMG_LoadTexture(renderer.fetch(), file.c_str());
	if(texture_ == nullptr) //TODO: gérer le cas pour les backgrounds
	{
		if((texture_ = IMG_LoadTexture(renderer.fetch(), "img/characters/image_not_found.png")) == nullptr)
		{
			SDL_Log("(IMG_LoadTexture) %s\n", IMG_GetError());
		}
	}
}

Texture::~Texture() //SDL_DestroyTexture
{
	SDL_DestroyTexture(texture_);
}

SDL_Texture* Texture::fetch() const
{
	return texture_;
}

void Texture::set_alpha_mod(Uint8 alpha)
{
	if(SDL_SetTextureAlphaMod(texture_, alpha) < 0)
	{
		SDL_Log("(SDL_SetTextureAlphaMod) %s\n", SDL_GetError());
	}
}

void Texture::set_color_mod(Uint8 r, Uint8 g, Uint8 b)
{
	if(SDL_SetTextureColorMod(texture_, r, g, b) < 0)
	{
		SDL_Log("(SDL_SetTextureColorMod) %s\n", SDL_GetError());
	}
}

void Texture::set_blend_mode(SDL_BlendMode blend_mode)
{
	if(SDL_SetTextureBlendMode(texture_, blend_mode) < 0)
	{
		SDL_Log("(SDL_SetTextureBlendMod) %s\n", SDL_GetError());
	}
}

void Texture::query(Uint32* format, int* access, int* w, int* h)
{
	if(SDL_QueryTexture(texture_, format, access, w, h) < 0)
	{
		SDL_Log("(SDL_QueryTexture) %s\n", SDL_GetError());
	}
}

}
