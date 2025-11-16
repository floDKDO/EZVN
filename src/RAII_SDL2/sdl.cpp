#include "RAII_SDL2/sdl.h"

#include <iostream>

namespace sdl
{

SDL::SDL(Uint32 flags)
{
	SDL_Init(flags);
	SDL_StartTextInput();
	SDL_GameControllerEventState(SDL_ENABLE);
}

SDL::~SDL()
{
	SDL_StopTextInput();
	SDL_Quit();
}

}