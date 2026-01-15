#pragma once

#include <SDL2/SDL.h>

namespace sdl::events
{

Uint32 register_events(int num_events);
void push_event(SDL_Event* event);

}