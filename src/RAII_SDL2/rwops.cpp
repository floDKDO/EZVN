#include "RAII_SDL2/rwops.h"

namespace sdl
{

RWops::RWops(const std::string file, const std::string mode) //SDL_RWFromFile
{
	rwops_ = SDL_RWFromFile(file.c_str(), mode.c_str());
}

RWops::~RWops() //SDL_RWclose
{
	if(rwops_ != nullptr)
		SDL_RWclose(rwops_);
}

SDL_RWops* RWops::Get() const
{
	return rwops_;
}

}