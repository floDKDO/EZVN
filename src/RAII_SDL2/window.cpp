#include "RAII_SDL2/window.h"

namespace sdl
{

Window::Window(std::string_view title, int x, int y, int w, int h, Uint32 flags) //SDL_CreateWindow
	: previous_x_(x), previous_y_(y), previous_width_(w), previous_height_(h)
{
	if((window_ = SDL_CreateWindow(title.data(), x, y, w, h, flags)) == nullptr)
	{
		SDL_Log("(SDL_CreateWindow) %s\n", SDL_GetError());
	}
}

Window::Window(Window&& window)
	: window_(window.window_)
{
	window.window_ = nullptr;
}

Window& Window::operator=(Window&& window)
{
	if(this == &window)
	{
		return *this;
	}

	if(window_ != nullptr)
	{
		SDL_DestroyWindow(window_);
	}

	window_ = window.window_;
	window.window_ = nullptr;
	return *this;
}

Window::~Window() //SDL_DestroyWindow
{
	if(window_ != nullptr)
	{
		SDL_DestroyWindow(window_);
	}
}

SDL_Window* Window::fetch() const
{
	return window_;
}

void Window::set_title(std::string_view title) const //SDL_SetWindowTitle
{
	SDL_SetWindowTitle(window_, title.data());
}

void Window::set_icon(sdl::Surface& icon) const //SDL_SetWindowIcon
{
	SDL_SetWindowIcon(window_, icon.fetch());
}

void Window::get_size(int* w, int* h) //SDL_GetWindowSize
{
	SDL_GetWindowSize(window_, w, h);
}

void Window::set_size(int w, int h) //SDL_SetWindowSize
{
	SDL_SetWindowSize(window_, w, h);
}

void Window::get_position(int* x, int* y) //SDL_GetWindowPosition
{
	SDL_GetWindowPosition(window_, x, y);
}

void Window::set_position(int x, int y) //SDL_SetWindowPosition
{
	SDL_SetWindowPosition(window_, x, y);
}

void Window::maximize() //SDL_MaximizeWindow
{
	get_size(&previous_width_, &previous_height_);
	get_position(&previous_x_, &previous_y_);
	SDL_MaximizeWindow(window_);
}

void Window::set_full_screen() //SDL_SetWindowFullscreen
{
	maximize();
	if(SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN) < 0)
	{
		SDL_Log("(SDL_SetWindowFullscreen) %s\n", SDL_GetError());
	}
}

void Window::set_windowed() //SDL_SetWindowFullscreen
{
	set_size(previous_width_, previous_height_);
	set_position(previous_x_, previous_y_);
	if(SDL_SetWindowFullscreen(window_, 0) < 0)
	{
		SDL_Log("(SDL_SetWindowFullscreen) %s\n", SDL_GetError());
	}
}

void Window::get_display_mode(SDL_DisplayMode* mode) //SDL_GetWindowDisplayMode
{
	if(SDL_GetWindowDisplayMode(window_, mode) < 0)
	{
		SDL_Log("(SDL_GetWindowDisplayMode) %s\n", SDL_GetError());
	}
}

void Window::set_display_mode(const SDL_DisplayMode* mode) //SDL_SetWindowDisplayMode
{
	if(SDL_SetWindowDisplayMode(window_, mode) < 0)
	{
		SDL_Log("(SDL_SetWindowDisplayMode) %s\n", SDL_GetError());
	}
}

}