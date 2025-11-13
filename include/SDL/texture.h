#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#include "SDL/renderer.h" //TODO!
#include "SDL/surface.h" //TODO!

namespace sdl
{
	class Texture;
}

class sdl::Texture
{
	public:
		Texture(SDL_Renderer* renderer, SDL_Surface* surface); //SDL_CreateTextureFromSurface()
		Texture(SDL_Renderer* renderer, const std::string file); //IMG_LoadTexture()
		Texture(const Texture& texture) = delete;
		Texture& operator=(const Texture& texture) = delete;
		~Texture(); //SDL_DestroyTexture

		SDL_Texture* Get() const;
		void set_alpha_mod(Uint8 alpha);
		void set_color_mod(Uint8 r, Uint8 g, Uint8 b);
		void set_blend_mode(SDL_BlendMode blendMode);
		void query(Uint32* format, int* access, int* w, int* h);

	protected:

	private:
		SDL_Texture* texture;
};

