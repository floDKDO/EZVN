#pragma once

#include <SDL2/SDL.h>

namespace sdl
{

class SDL
{
	public:
		explicit SDL(Uint32 flags);
		SDL(const SDL& sdl) = delete;
		SDL(SDL&& sdl) = delete;
		SDL& operator=(const SDL& sdl) = delete;
		SDL& operator=(SDL&& sdl) = delete;
		~SDL();

};
}
