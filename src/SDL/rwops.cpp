#include "SDL/rwops.h"

namespace sdl
{

RWops::RWops(const std::string file, const std::string mode) //SDL_RWFromFile
{
	this->rwops = SDL_RWFromFile(file.c_str(), mode.c_str());
}

RWops::~RWops() //SDL_RWclose
{
	SDL_RWclose(this->rwops);
}

SDL_RWops* RWops::Get() const
{
	return this->rwops;
}

}