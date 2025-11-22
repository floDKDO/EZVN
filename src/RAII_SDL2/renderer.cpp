#include "RAII_SDL2/renderer.h"
#include "RAII_SDL2/texture.h"

#include <iostream>

namespace sdl
{

Renderer::Renderer(Window& window, int index, Uint32 flags) //SDL_CreateRenderer()
{
	if((renderer_ = SDL_CreateRenderer(window.Get(), index, flags)) == nullptr)
	{
		SDL_Log("(SDL_CreateRenderer) %s\n", SDL_GetError());
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
}

Renderer::~Renderer() //SDL_DestroyRenderer
{
	SDL_DestroyRenderer(renderer_);
}

SDL_Renderer* Renderer::Get() const
{
	return renderer_;
}

void Renderer::copy(Texture& texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect)
{
	if(SDL_RenderCopy(renderer_, texture.Get(), srcrect, dstrect) < 0)
	{
		SDL_Log("(SDL_RenderCopy) %s\n", SDL_GetError());
	}
}

void Renderer::copy(Texture& texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, const double angle, const SDL_Point* center, const SDL_RendererFlip flip)
{
	if(SDL_RenderCopyEx(renderer_, texture.Get(), srcrect, dstrect, angle, center, flip) < 0)
	{
		SDL_Log("(SDL_RenderCopyEx) %s\n", SDL_GetError());
	}
}

void Renderer::clear()
{
	if(SDL_RenderClear(renderer_) < 0)
	{
		SDL_Log("(SDL_RenderClear) %s\n", SDL_GetError());
	}
}

void Renderer::present()
{
	SDL_RenderPresent(renderer_);
}

void Renderer::set_logical_size(int w, int h)
{
	if(SDL_RenderSetLogicalSize(renderer_, w, h) < 0)
	{
		SDL_Log("(SDL_RenderSetLogicalSize) %s\n", SDL_GetError());
	}
}

void Renderer::set_draw_blend_mode(SDL_BlendMode blendMode)
{
	if(SDL_SetRenderDrawBlendMode(renderer_, blendMode) < 0)
	{
		SDL_Log("(SDL_SetRenderDrawBlendMode) %s\n", SDL_GetError());
	}
}

}
