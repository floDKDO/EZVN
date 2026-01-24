#pragma once

#include <SDL2/SDL.h>
#include <string_view>

namespace sdl
{

class RWops
{
	public:
		RWops(std::string_view file, std::string_view mode); //SDL_RWFromFile
		RWops(const RWops& rwops) = delete;
		RWops(RWops&& rwops);
		RWops& operator=(const RWops& rwops) = delete;
		RWops& operator=(RWops&& rwops);
		~RWops(); //SDL_RWclose

		SDL_RWops* fetch() const;
		bool is_gif() const;
		bool is_webp() const;
		bool is_animation() const;

	private:
		SDL_RWops* rwops_;
};
}
