#include "RAII_SDL2/sdl_img.h"

#include <SDL2/SDL_image.h>

namespace sdl
{

SDLImage::SDLImage(int flags)
{
	if(IMG_Init(flags) == 0)
	{
		SDL_Log("(IMG_Init) %s\n", IMG_GetError());
	}
}

SDLImage::~SDLImage()
{
	IMG_Quit();
}

}