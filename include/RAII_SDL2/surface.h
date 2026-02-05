#pragma once

#include "RAII_SDL2/font.h"

#include <SDL2/SDL.h>

namespace sdl
{

class Surface
{
	public:
		Surface(Font& font, std::string_view text, SDL_Color fg, Uint32 wrap_length); //TTF_RenderUTF8_Blended_Wrapped();
		explicit Surface(std::string_view file); //IMG_Load
		Surface(int width, int height); //SDL_CreateRGBSurfaceWithFormat
		Surface(const Surface& surface) = delete;
		Surface(Surface&& surface);
		Surface& operator=(const Surface& surface) = delete;
		Surface& operator=(Surface&& surface);
		~Surface(); //SDL_FreeSurface

		SDL_Surface* fetch() const;
		void set_blend_mode(SDL_BlendMode blend_mode) const;
		void blit(const SDL_Rect* srcrect, Surface& dst, SDL_Rect* dstrect) const;

	private:
		SDL_Surface* surface_;
};

}

