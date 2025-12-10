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

RWops::RWops(RWops&& rwops)
	: rwops_(rwops.rwops_)
{
	rwops.rwops_ = nullptr;
}

RWops& RWops::operator=(RWops&& rwops)
{
	if(this == &rwops)
	{
		return *this;
	}

	if(rwops_ != nullptr)
	{
		if(SDL_RWclose(rwops_) < 0)
		{
			SDL_Log("(SDL_RWclose) %s\n", SDL_GetError());
		}
	}

	rwops_ = rwops.rwops_;
	rwops.rwops_ = nullptr;
	return *this;
}

RWops::~RWops() //SDL_RWclose
{
	if(rwops_ != nullptr)
	{
		if(SDL_RWclose(rwops_) < 0)
		{
			SDL_Log("(SDL_RWclose) %s\n", SDL_GetError());
		}
	}
}

SDL_RWops* RWops::fetch() const
{
	return rwops_;
}

}