#pragma once

#include <SDL2/SDL_image.h>

namespace sdl
{

class SDLImage
{
	public:
		explicit SDLImage(const int flags);
		SDLImage(const SDLImage& sdl) = delete;
		SDLImage& operator=(const SDLImage& sdl) = delete;
		~SDLImage();

};
}
