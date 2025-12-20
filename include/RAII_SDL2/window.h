#pragma once

#include "RAII_SDL2/surface.h"

#include <SDL2/SDL.h>
#include <string>

namespace sdl
{

class Window
{
	public:
		Window(const std::string_view title, const int x, const int y, const int w, const int h, const Uint32 flags); //SDL_CreateWindow
		Window(const Window& window) = delete;
		Window(Window&& window);
		Window& operator=(const Window& window) = delete;
		Window& operator=(Window&& window);
		~Window(); //SDL_DestroyWindow

		SDL_Window* fetch() const;
		void set_title(const std::string_view title) const; //SDL_SetWindowTitle
		void set_icon(sdl::Surface& icon) const; //SDL_SetWindowIcon
		void get_size(int* w, int* h); //SDL_GetWindowSize
		void set_size(const int w, const int h); //SDL_SetWindowSize
		void get_position(int* x, int* y); //SDL_GetWindowPosition
		void set_position(const int x, const int y); //SDL_SetWindowPosition
		void maximize(); //SDL_MaximizeWindow
		void set_full_screen(); //SDL_SetWindowFullscreen
		void set_windowed(); //SDL_SetWindowFullscreen

	private:
		SDL_Window* window_;
		int previous_x_, previous_y_;
		int previous_width_, previous_height_;
};
}

