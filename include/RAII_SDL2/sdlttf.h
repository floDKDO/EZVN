#pragma once

#include <SDL2/SDL_ttf.h>

namespace sdl
{

class SDLTTF
{
	public:
		SDLTTF();
		SDLTTF(const SDLTTF& sdl_ttf) = delete;
		SDLTTF(SDLTTF&& sdl_ttf) = delete;
		SDLTTF& operator=(const SDLTTF& sdl_ttf) = delete;
		SDLTTF& operator=(SDLTTF&& sdl_ttf) = delete;
		~SDLTTF();

};
}

