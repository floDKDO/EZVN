#include "RAII_SDL2/rwops.h"

namespace sdl
{

RWops::RWops(const std::string_view file, const std::string_view mode) //SDL_RWFromFile
{
	if((rwops_ = SDL_RWFromFile(file.data(), mode.data())) == nullptr)
	{
		SDL_Log("(SDL_RWFromFile) %s\n", SDL_GetError());
	}
}

RWops::~RWops() //SDL_RWclose
{
	if(SDL_RWclose(rwops_) < 0)
	{
		SDL_Log("(SDL_RWclose) %s\n", SDL_GetError());
	}
}

SDL_RWops* RWops::fetch() const
{
	return rwops_;
}

}