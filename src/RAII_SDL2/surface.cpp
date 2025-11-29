#include "RAII_SDL2/surface.h"

#include <SDL2/SDL_image.h>

namespace sdl
{

Surface::Surface(Font& font, const std::string text, SDL_Color fg, Uint32 wrapLength)
{
	if((surface_ = TTF_RenderUTF8_Blended_Wrapped(font.Get(), text.c_str(), fg, wrapLength)) == nullptr)
	{
		SDL_Log("(TTF_RenderUTF8_Blended_Wrapped) %s\n", TTF_GetError());
	}
}

Surface::Surface(const std::string file)
{
	if((surface_ = IMG_Load(file.c_str())) == nullptr)
	{
		SDL_Log("(IMG_Load) %s\n", IMG_GetError());
	}
}

Surface::~Surface() //SDL_FreeSurface
{
	SDL_FreeSurface(surface_);
}

SDL_Surface* Surface::Get() const
{
	return surface_;
}

void Surface::set_blend_mode(SDL_BlendMode blendMode)
{
	if(SDL_SetSurfaceBlendMode(surface_, blendMode) < 0)
	{
		SDL_Log("(SDL_SetSurfaceBlendMode) %s\n", SDL_GetError());
	}
}

void Surface::blit(SDL_Rect* srcrect, Surface& dst, SDL_Rect* dstrect)
{
	if(SDL_BlitSurface(surface_, srcrect, dst.Get(), dstrect) < 0)
	{
		SDL_Log("(SDL_BlitSurface) %s\n", SDL_GetError());
	}
}

}
