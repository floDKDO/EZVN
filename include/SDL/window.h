#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace sdl
{
	class Window;
}

class sdl::Window
{
	public:
		Window(const std::string title, int x, int y, int w, int h, Uint32 flags); //SDL_CreateWindow
		Window(const Window& window); 
		Window& operator=(const Window& window);
		~Window(); //SDL_DestroyWindow

		SDL_Window* Get() const;
		void set_title(const std::string title);

	protected:

	private:
		SDL_Window* window;
};

