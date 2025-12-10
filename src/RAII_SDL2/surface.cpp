#include "RAII_SDL2/surface.h"

#include <SDL2/SDL_image.h>

namespace sdl
{

Surface::Surface(Font& font, const std::string_view text, const SDL_Color fg, const Uint32 wrap_length)
{
	if((surface_ = TTF_RenderUTF8_Blended_Wrapped(font.fetch(), text.data(), fg, wrap_length)) == nullptr)
	{
		SDL_Log("(TTF_RenderUTF8_Blended_Wrapped) %s\n", TTF_GetError());
	}
}

Surface::Surface(const std::string_view file)
{
	if((surface_ = IMG_Load(file.data())) == nullptr)
	{
		SDL_Log("(IMG_Load) %s\n", IMG_GetError());
	}
}

Surface::Surface(Surface&& surface)
	: surface_(surface.surface_)
{
	surface.surface_ = nullptr;
}

Surface& Surface::operator=(Surface&& surface)
{
	if(this == &surface)
	{
		return *this;
	}

	if(surface_ != nullptr)
	{
		SDL_FreeSurface(surface_);
	}

	surface_ = surface.surface_;
	surface.surface_ = nullptr;
	return *this;
}

Surface::~Surface() //SDL_FreeSurface
{
	if(surface_ != nullptr)
	{
		SDL_FreeSurface(surface_);
	}
}

SDL_Surface* Surface::fetch() const
{
	return surface_;
}

void Surface::set_blend_mode(const SDL_BlendMode blend_mode) const
{
	if(SDL_SetSurfaceBlendMode(surface_, blend_mode) < 0)
	{
		SDL_Log("(SDL_SetSurfaceBlendMode) %s\n", SDL_GetError());
	}
}

void Surface::blit(SDL_Rect* srcrect, Surface& dst, SDL_Rect* dstrect) const
{
	if(SDL_BlitSurface(surface_, srcrect, dst.fetch(), dstrect) < 0)
	{
		SDL_Log("(SDL_BlitSurface) %s\n", SDL_GetError());
	}
}

}
