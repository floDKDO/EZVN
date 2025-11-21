#pragma once

#include <SDL2/SDL.h>

namespace sdl
{

class SDL
{
	public:
	SDL(Uint32 flags);
	SDL(const SDL& sdl) = delete;
	SDL& operator=(const SDL& sdl) = delete;
	~SDL();

	protected:

	private:

};
}
