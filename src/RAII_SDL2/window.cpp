#include "RAII_SDL2/window.h"

namespace sdl
{

Window::Window(const std::string title, int x, int y, int w, int h, Uint32 flags) //SDL_CreateWindow
{
	if((window_ = SDL_CreateWindow(title.c_str(), x, y, w, h, flags)) == nullptr)
	{
		SDL_Log("(SDL_CreateWindow) %s\n", SDL_GetError());
	}
}

Window::~Window() //SDL_DestroyWindow
{
	SDL_DestroyWindow(window_);
}

SDL_Window* Window::Get() const
{
	return window_;
}

void Window::set_title(const std::string title)
{
	SDL_SetWindowTitle(window_, title.c_str());
}

void Window::set_icon(sdl::Surface& icon)
{
	SDL_SetWindowIcon(window_, icon.Get());
}

void Window::set_full_screen()
{
	if(SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN) < 0)
	{
		SDL_Log("(SDL_SetWindowFullscreen) %s\n", SDL_GetError());
	}
}

void Window::set_windowed()
{
	if(SDL_SetWindowFullscreen(window_, 0) < 0)
	{
		SDL_Log("(SDL_SetWindowFullscreen) %s\n", SDL_GetError());
	}
}

}