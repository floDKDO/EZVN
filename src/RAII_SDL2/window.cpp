#include "RAII_SDL2/window.h"

namespace sdl
{

Window::Window(const std::string title, int x, int y, int w, int h, Uint32 flags) //SDL_CreateWindow
{
	window_ = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
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

void Window::set_full_screen()
{
	SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
}

void Window::set_windowed()
{
	SDL_SetWindowFullscreen(window_, 0);
}

}