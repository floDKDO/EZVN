#pragma once

#include "RAII_SDL2/font.h"

#include <SDL2/SDL.h>

namespace sdl
{

class Surface
{
	public:
		Surface(Font& font, const std::string text, SDL_Color fg, Uint32 wrap_length); //TTF_RenderUTF8_Blended_Wrapped();
		explicit Surface(const std::string file); //IMG_Load
		Surface(const Surface& surface) = delete;
		Surface& operator=(const Surface& surface) = delete;
		~Surface(); //SDL_FreeSurface

		SDL_Surface* fetch() const;
		void set_blend_mode(SDL_BlendMode blend_mode);
		void blit(SDL_Rect* srcrect, Surface& dst, SDL_Rect* dstrect);

	protected:

	private:
		SDL_Surface* surface_;
};

}

