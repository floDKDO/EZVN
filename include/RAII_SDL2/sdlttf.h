#pragma once

#include <SDL2/SDL_ttf.h>

namespace sdl
{

class SDLTTF
{
	public:
		SDLTTF();
		SDLTTF(const SDLTTF& sdl) = delete;
		SDLTTF& operator=(const SDLTTF& sdl) = delete;
		~SDLTTF();

};
}

