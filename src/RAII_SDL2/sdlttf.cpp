#include "RAII_SDL2/sdlttf.h"

namespace sdl
{

SDLTTF::SDLTTF()
{
	if(TTF_Init() == -1)
	{
		SDL_Log("(TTF_Init) %s\n", TTF_GetError());
	}
}

SDLTTF::~SDLTTF()
{
	TTF_Quit();
}

}