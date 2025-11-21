#include "RAII_SDL2/texture.h"
#include "RAII_SDL2/renderer.h"
#include <iostream>

namespace sdl
{

Texture::Texture(Renderer& renderer, Surface& surface) //SDL_CreateTextureFromSurface()
{
	this->texture = SDL_CreateTextureFromSurface(renderer.Get(), surface.Get());
}

Texture::Texture(Renderer& renderer, SDL_Surface* surface) //SDL_CreateTextureFromSurface() with surface a frame from a IMG_Animation*
{
	this->texture = SDL_CreateTextureFromSurface(renderer.Get(), surface);
}

Texture::Texture(Renderer& renderer, const std::string file) //IMG_LoadTexture()
{
	this->texture = IMG_LoadTexture(renderer.Get(), file.c_str());
	if(this->texture == nullptr) //TODO: gérer le cas pour les backgrounds
	{
		this->texture = IMG_LoadTexture(renderer.Get(), "img/characters/image_not_found.png");
	}
}

Texture::~Texture() //SDL_DestroyTexture
{
	SDL_DestroyTexture(this->texture);
}

SDL_Texture* Texture::Get() const
{
	return this->texture;
}

void Texture::set_alpha_mod(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(this->texture, alpha);
}

void Texture::set_color_mod(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetTextureColorMod(this->texture, r, g, b);
}

void Texture::set_blend_mode(SDL_BlendMode blendMode)
{
	SDL_SetTextureBlendMode(this->texture, blendMode);
}

void Texture::query(Uint32* format, int* access, int* w, int* h)
{
	SDL_QueryTexture(this->texture, format, access, w, h);
}

}
