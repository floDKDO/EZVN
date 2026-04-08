#include "RAII_SDL2/pref_path.h"
#include "constants.h"

#include <SDL2/SDL.h>

namespace sdl
{

PrefPath::PrefPath() //SDL_GetPrefPath()
{
	if((pref_path_ = SDL_GetPrefPath(nullptr, "EZVN")) == nullptr)
	{
		SDL_Log("(SDL_GetPrefPath) %s\n", SDL_GetError());
	}
}

PrefPath::~PrefPath() //SDL_free()
{
	SDL_free(pref_path_);
}

std::string PrefPath::get_pref_path() const
{
	return std::string(pref_path_);
}

}