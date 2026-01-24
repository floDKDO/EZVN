#pragma once

namespace sdl
{

class SDLImage
{
	public:
		explicit SDLImage(int flags);
		SDLImage(const SDLImage& sdl_image) = delete;
		SDLImage(SDLImage&& sdl_image) = delete;
		SDLImage& operator=(const SDLImage& sdl_image) = delete;
		SDLImage& operator=(SDLImage&& sdl_image) = delete;
		~SDLImage();

};
}
