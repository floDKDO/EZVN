#pragma once

#include <SDL2/SDL.h>

namespace sdl::display
{

int get_num_display_modes(); //implicit display_index = 0
void get_display_mode(int mode_index, SDL_DisplayMode* mode); //implicit display_index = 0

}

