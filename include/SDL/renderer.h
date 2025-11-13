#pragma once

#include <SDL2/SDL.h>

namespace sdl
{
	 class Renderer;
}

class sdl::Renderer
{
	public:
		Renderer(SDL_Window* window, int index, Uint32 flags); //SDL_CreateRenderer()
		Renderer(const Renderer& renderer) = delete;
		Renderer& operator=(const Renderer& renderer) = delete;
		~Renderer(); //SDL_DestroyRenderer

		SDL_Renderer* Get() const;

		void clear();
		void present();
		void set_logical_size(int w, int h);
		void set_draw_blend_mode(SDL_BlendMode blendMode);

	protected:

	private:
		SDL_Renderer* renderer;
};

