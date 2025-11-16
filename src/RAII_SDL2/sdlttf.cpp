#include "RAII_SDL2/sdlttf.h"

namespace sdl
{

SDLTTF::SDLTTF()
{
	TTF_Init();
}

SDLTTF::~SDLTTF()
{
	TTF_Quit();
}

}