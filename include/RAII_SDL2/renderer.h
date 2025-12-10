#pragma once

#include "RAII_SDL2/window.h"

namespace sdl
{

class Texture;

class Renderer
{
	public:
		Renderer(Window& window, const int index, const Uint32 flags); //SDL_CreateRenderer()
		Renderer(const Renderer& renderer) = delete;
		Renderer(Renderer&& renderer);
		Renderer& operator=(const Renderer& renderer) = delete;
		Renderer& operator=(Renderer&& renderer);
		~Renderer(); //SDL_DestroyRenderer

		SDL_Renderer* fetch() const;

		void copy(Texture& texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) const;
		void copy(Texture& texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, const double angle, const SDL_Point* center, const SDL_RendererFlip flip) const;
		void clear() const;
		void present() const;
		void set_logical_size(const int w, const int h) const;
		void set_draw_blend_mode(const SDL_BlendMode blend_mode) const;

	private:
		SDL_Renderer* renderer_;
};
}
