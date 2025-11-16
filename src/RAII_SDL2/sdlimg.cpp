#include "RAII_SDL2/sdlimg.h"

namespace sdl
{

SDLImage::SDLImage(int flags)
{
	IMG_Init(flags);
}

SDLImage::~SDLImage()
{
	IMG_Quit();
}

}