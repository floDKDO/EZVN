#include "SDL/texture.h"

namespace sdl
{

Texture::Texture(SDL_Renderer* renderer, SDL_Surface* surface) //SDL_CreateTextureFromSurface()
{
	this->texture = SDL_CreateTextureFromSurface(renderer, surface);
}

Texture::Texture(SDL_Renderer* renderer, const std::string file) //IMG_LoadTexture()
{
	this->texture = IMG_LoadTexture(renderer, file.c_str());
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
