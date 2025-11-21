#include "RAII_SDL2/renderer.h"
#include "RAII_SDL2/texture.h"

#include <iostream>

namespace sdl
{

Renderer::Renderer(Window& window, int index, Uint32 flags) //SDL_CreateRenderer()
{
	renderer_ = SDL_CreateRenderer(window.Get(), index, flags);
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
	SDL_RenderCopy(renderer_, texture.Get(), srcrect, dstrect);
}

void Renderer::copy(Texture& texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, const double angle, const SDL_Point* center, const SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(renderer_, texture.Get(), srcrect, dstrect, angle, center, flip);
}

void Renderer::clear()
{
	SDL_RenderClear(renderer_);
}

void Renderer::present()
{
	SDL_RenderPresent(renderer_);
}

void Renderer::set_logical_size(int w, int h)
{
	SDL_RenderSetLogicalSize(renderer_, w, h);
}

void Renderer::set_draw_blend_mode(SDL_BlendMode blendMode)
{
	SDL_SetRenderDrawBlendMode(renderer_, blendMode);
}

}
