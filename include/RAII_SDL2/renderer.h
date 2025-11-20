#pragma once

#include <SDL2/SDL.h>

#include "RAII_SDL2/window.h"

class Texture;

namespace sdl
{
	 class Renderer;
}

class sdl::Renderer
{
	public:
		Renderer(Window& window, int index, Uint32 flags); //SDL_CreateRenderer()
		Renderer(const Renderer& renderer) = delete;
		Renderer& operator=(const Renderer& renderer) = delete;
		~Renderer(); //SDL_DestroyRenderer

		SDL_Renderer* Get() const;

		void copy(Texture& texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
		void copy(Texture& texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, const double angle, const SDL_Point* center, const SDL_RendererFlip flip);
		void clear();
		void present();
		void set_logical_size(int w, int h);
		void set_draw_blend_mode(SDL_BlendMode blendMode);

	protected:

	private:
		SDL_Renderer* renderer;
};

