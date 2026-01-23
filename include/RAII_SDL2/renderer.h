#pragma once

#include "RAII_SDL2/window.h"

namespace sdl
{

class Texture;

class Renderer
{
	public:
		Renderer(Window& window, int index, Uint32 flags); //SDL_CreateRenderer()
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
		void set_logical_size(int w, int h) const;
		void set_draw_blend_mode(SDL_BlendMode blend_mode) const;
		void set_draw_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		void fill_rect(const SDL_Rect* rect);
		void draw_rect(const SDL_Rect* rect);
		void window_to_logical(int window_x, int window_y, float* logical_x, float* logical_y);
		void set_clip_rect(const SDL_Rect * rect);

	private:
		SDL_Renderer* renderer_;
};
}
