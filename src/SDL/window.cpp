#include "SDL/window.h"

namespace sdl
{

Window::Window(const std::string title, int x, int y, int w, int h, Uint32 flags) //SDL_CreateWindow
{
	this->window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
}

Window::Window(const Window& window)
{
	//TODO!
}

Window& Window::operator=(const Window& window)
{
	//TODO!
	return *this;
}

Window::~Window() //SDL_DestroyWindow
{
	SDL_DestroyWindow(this->window);
}

SDL_Window* Window::Get() const
{
	return this->window;
}

void Window::set_title(const std::string title)
{
	SDL_SetWindowTitle(this->window, title.c_str());
}

}