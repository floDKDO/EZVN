#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

namespace sdl
{
	class Surface;
}

class sdl::Surface
{
	public:
		Surface(TTF_Font* font, const std::string text, SDL_Color fg, Uint32 wrapLength); //TTF_RenderUTF8_Blended_Wrapped();
		Surface(const Surface& surface) = delete;
		Surface& operator=(const Surface& surface) = delete;
		~Surface(); //SDL_FreeSurface

		SDL_Surface* Get() const;

	protected:

	private:
		SDL_Surface* surface;
};

