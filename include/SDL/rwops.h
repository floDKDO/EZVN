#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace sdl
{
	class RWops;
}

class sdl::RWops
{
	public:
		RWops(const std::string file, const std::string mode); //SDL_RWFromFile
		RWops(const RWops& rwops);
		RWops& operator=(const RWops& rwops);
		~RWops(); //SDL_RWclose

		SDL_RWops* Get() const;

	protected:

	private:
		SDL_RWops* rwops;
};

