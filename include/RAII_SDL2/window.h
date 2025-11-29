#pragma once

#include "RAII_SDL2/surface.h"

#include <SDL2/SDL.h>
#include <string>

namespace sdl
{

class Window
{
	public:
		Window(const std::string title, int x, int y, int w, int h, Uint32 flags); //SDL_CreateWindow
		Window(const Window& window) = delete;
		Window& operator=(const Window& window) = delete;
		~Window(); //SDL_DestroyWindow

		SDL_Window* Get() const;
		void set_title(const std::string title);
		void set_icon(sdl::Surface& icon);
		void set_full_screen();
		void set_windowed();

	protected:

	private:
		SDL_Window* window_;
};
}

