#include "RAII_SDL2/display.h"

namespace sdl::display
{

int get_num_display_modes()
{
	int num;
	if((num = SDL_GetNumDisplayModes(0)) < 0)
	{
		SDL_Log("(SDL_GetNumDisplayModes) %s\n", SDL_GetError());
	}
	return num;
}

void get_display_mode(int mode_index, SDL_DisplayMode* mode)
{
	if(SDL_GetDisplayMode(0, mode_index, mode) < 0)
	{
		SDL_Log("(SDL_GetDisplayMode) %s\n", SDL_GetError());
	}
}

}