#pragma once

#include "RAII_SDL2/surface.h"

namespace sdl
{

class Renderer;

class Texture
{
	public:
		Texture(Renderer& renderer, Surface& surface); //SDL_CreateTextureFromSurface()
		Texture(Renderer& renderer, SDL_Surface* surface); //SDL_CreateTextureFromSurface() with surface a frame from a IMG_Animation* => surface is not owned by the Texture class!!
		Texture(Renderer& renderer, const std::string_view file); //IMG_LoadTexture()
		Texture(const Texture& texture) = delete;
		Texture(Texture&& texture);
		Texture& operator=(const Texture& texture) = delete;
		Texture& operator=(Texture&& texture);
		~Texture(); //SDL_DestroyTexture

		SDL_Texture* fetch() const;
		void set_alpha_mod(const Uint8 alpha) const;
		void set_color_mod(const Uint8 r, const Uint8 g, const Uint8 b) const;
		void set_blend_mode(const SDL_BlendMode blend_mode) const;
		void query(Uint32* format, int* access, int* w, int* h) const;

	private:
		SDL_Texture* texture_;
};
}
