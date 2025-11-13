#include "SDL/renderer.h"

namespace sdl
{

Renderer::Renderer(SDL_Window* window, int index, Uint32 flags) //SDL_CreateRenderer()
{
	this->renderer = SDL_CreateRenderer(window, index, flags);
}

Renderer::~Renderer() //SDL_DestroyRenderer
{
	SDL_DestroyRenderer(this->renderer);
}

SDL_Renderer* Renderer::Get() const
{
	return this->renderer;
}

void Renderer::clear()
{
	SDL_RenderClear(this->renderer);
}

void Renderer::present()
{
	SDL_RenderPresent(this->renderer);
}

void Renderer::set_logical_size(int w, int h)
{
	SDL_RenderSetLogicalSize(this->renderer, w, h);
}

void Renderer::set_draw_blend_mode(SDL_BlendMode blendMode)
{
	SDL_SetRenderDrawBlendMode(this->renderer, blendMode);
}

}
