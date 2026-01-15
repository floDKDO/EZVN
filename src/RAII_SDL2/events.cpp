#include "RAII_SDL2/events.h"

namespace sdl::events
{

Uint32 register_events(int num_events)
{
	return SDL_RegisterEvents(num_events);
}

void push_event(SDL_Event* event)
{
	if(SDL_PushEvent(event) < 0)
	{
		SDL_Log("(SDL_PushEvent) %s\n", SDL_GetError());
	}
}

}