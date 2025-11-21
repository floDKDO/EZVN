#include "RAII_SDL2/surface.h"

namespace sdl
{

Surface::Surface(Font& font, const std::string text, SDL_Color fg, Uint32 wrapLength)
{
	this->surface = TTF_RenderUTF8_Blended_Wrapped(font.Get(), text.c_str(), fg, wrapLength);
}

Surface::~Surface() //SDL_FreeSurface
{
	SDL_FreeSurface(this->surface);
}

SDL_Surface* Surface::Get() const
{
	return this->surface;
}

}
