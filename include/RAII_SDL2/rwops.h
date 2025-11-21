#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace sdl
{

class RWops
{
	public:
	RWops(const std::string file, const std::string mode); //SDL_RWFromFile
	RWops(const RWops& rwops) = delete;
	RWops& operator=(const RWops& rwops) = delete;
	~RWops(); //SDL_RWclose

	SDL_RWops* Get() const;

	protected:

	private:
	SDL_RWops* rwops_;
};
}
