#pragma once

#include "RAII_SDL2/surface.h"

#include <SDL2/SDL.h>
#include <string>

namespace sdl
{

class Window
{
	public:
		Window(const std::string title, const int x, const int y, const int w, const int h, const Uint32 flags); //SDL_CreateWindow
		Window(const Window& window) = delete;
		Window& operator=(const Window& window) = delete;
		~Window(); //SDL_DestroyWindow

		SDL_Window* fetch() const;
		void set_title(const std::string title) const;
		void set_icon(sdl::Surface& icon) const;
		void set_full_screen() const;
		void set_windowed() const;

	private:
		SDL_Window* window_;
};
}

