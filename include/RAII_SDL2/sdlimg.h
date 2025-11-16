#pragma once

#include <SDL2/SDL_image.h>

namespace sdl
{
	class SDLImage;
}

class sdl::SDLImage
{
	public:
		SDLImage(int flags);
		SDLImage(const SDLImage& sdl) = delete;
		SDLImage& operator=(const SDLImage& sdl) = delete;
		~SDLImage();

	protected:

	private:

};

