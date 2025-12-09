#pragma once

#include "RAII_SDL2/font.h"

#include <SDL2/SDL.h>

namespace sdl
{

class Surface
{
	public:
		Surface(Font& font, const std::string text, const SDL_Color fg, const Uint32 wrap_length); //TTF_RenderUTF8_Blended_Wrapped();
		explicit Surface(const std::string_view file); //IMG_Load
		Surface(const Surface& surface) = delete;
		Surface& operator=(const Surface& surface) = delete;
		~Surface(); //SDL_FreeSurface

		SDL_Surface* fetch() const;
		void set_blend_mode(const SDL_BlendMode blend_mode) const;
		void blit(SDL_Rect* srcrect, Surface& dst, SDL_Rect* dstrect) const;

	private:
		SDL_Surface* surface_;
};

}

