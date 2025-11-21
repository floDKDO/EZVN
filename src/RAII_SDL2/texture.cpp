#include "RAII_SDL2/texture.h"
#include "RAII_SDL2/renderer.h"

#include <SDL2/SDL_image.h>
#include <iostream>

namespace sdl
{

Texture::Texture(Renderer& renderer, Surface& surface) //SDL_CreateTextureFromSurface()
{
	texture_ = SDL_CreateTextureFromSurface(renderer.Get(), surface.Get());
}

Texture::Texture(Renderer& renderer, SDL_Surface* surface) //SDL_CreateTextureFromSurface() with surface a frame from a IMG_Animation*
{
	texture_ = SDL_CreateTextureFromSurface(renderer.Get(), surface);
}

Texture::Texture(Renderer& renderer, const std::string file) //IMG_LoadTexture()
{
	texture_ = IMG_LoadTexture(renderer.Get(), file.c_str());
	if(texture_ == nullptr) //TODO: gérer le cas pour les backgrounds
	{
		texture_ = IMG_LoadTexture(renderer.Get(), "img/characters/image_not_found.png");
	}
}

Texture::~Texture() //SDL_DestroyTexture
{
	SDL_DestroyTexture(texture_);
}

SDL_Texture* Texture::Get() const
{
	return texture_;
}

void Texture::set_alpha_mod(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(texture_, alpha);
}

void Texture::set_color_mod(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetTextureColorMod(texture_, r, g, b);
}

void Texture::set_blend_mode(SDL_BlendMode blendMode)
{
	SDL_SetTextureBlendMode(texture_, blendMode);
}

void Texture::query(Uint32* format, int* access, int* w, int* h)
{
	SDL_QueryTexture(texture_, format, access, w, h);
}

}
