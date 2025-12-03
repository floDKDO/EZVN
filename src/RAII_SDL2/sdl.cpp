#include "RAII_SDL2/sdl.h"

#include <iostream>

namespace sdl
{

SDL::SDL(const Uint32 flags)
{
	if(SDL_Init(flags) < 0)
	{
		SDL_Log("(SDL_Init) %s\n", SDL_GetError());
	}
	SDL_StartTextInput();
	SDL_GameControllerEventState(SDL_ENABLE);
}

SDL::~SDL()
{
	SDL_StopTextInput();
	SDL_Quit();
}

}