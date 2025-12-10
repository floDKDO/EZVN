#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace sdl
{

class RWops
{
	public:
		RWops(const std::string_view file, const std::string_view mode); //SDL_RWFromFile
		RWops(const RWops& rwops) = delete;
		RWops(RWops&& rwops);
		RWops& operator=(const RWops& rwops) = delete;
		RWops& operator=(RWops&& rwops);
		~RWops(); //SDL_RWclose

		SDL_RWops* fetch() const;

	private:
		SDL_RWops* rwops_;
};
}
