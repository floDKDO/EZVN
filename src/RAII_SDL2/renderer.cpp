#include "RAII_SDL2/renderer.h"
#include "RAII_SDL2/texture.h"

#include <iostream>

namespace sdl
{

Renderer::Renderer(Window& window, const int index, const Uint32 flags) //SDL_CreateRenderer()
{
	if((renderer_ = SDL_CreateRenderer(window.fetch(), index, flags)) == nullptr)
	{
		SDL_Log("(SDL_CreateRenderer) %s\n", SDL_GetError());
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
}

Renderer::Renderer(Renderer&& renderer)
	: renderer_(renderer.renderer_)
{
	renderer.renderer_ = nullptr;
}

Renderer& Renderer::operator=(Renderer&& renderer)
{
	if(this == &renderer)
	{
		return *this;
	}

	if(renderer_ != nullptr)
	{
		SDL_DestroyRenderer(renderer_);
	}

	renderer_ = renderer.renderer_;
	renderer.renderer_ = nullptr;
	return *this;
}

Renderer::~Renderer() //SDL_DestroyRenderer
{
	if(renderer_ != nullptr)
	{
		SDL_DestroyRenderer(renderer_);
	}
}

SDL_Renderer* Renderer::fetch() const
{
	return renderer_;
}

void Renderer::copy(Texture& texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) const
{
	if(SDL_RenderCopy(renderer_, texture.fetch(), srcrect, dstrect) < 0)
	{
		SDL_Log("(SDL_RenderCopy) %s\n", SDL_GetError());
	}
}

void Renderer::copy(Texture& texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, const double angle, const SDL_Point* center, const SDL_RendererFlip flip) const
{
	if(SDL_RenderCopyEx(renderer_, texture.fetch(), srcrect, dstrect, angle, center, flip) < 0)
	{
		SDL_Log("(SDL_RenderCopyEx) %s\n", SDL_GetError());
	}
}

void Renderer::clear() const
{
	if(SDL_RenderClear(renderer_) < 0)
	{
		SDL_Log("(SDL_RenderClear) %s\n", SDL_GetError());
	}
}

void Renderer::present() const
{
	SDL_RenderPresent(renderer_);
}

void Renderer::set_logical_size(const int w, const int h) const
{
	if(SDL_RenderSetLogicalSize(renderer_, w, h) < 0)
	{
		SDL_Log("(SDL_RenderSetLogicalSize) %s\n", SDL_GetError());
	}
}

void Renderer::set_draw_blend_mode(const SDL_BlendMode blend_mode) const
{
	if(SDL_SetRenderDrawBlendMode(renderer_, blend_mode) < 0)
	{
		SDL_Log("(SDL_SetRenderDrawBlendMode) %s\n", SDL_GetError());
	}
}

}
