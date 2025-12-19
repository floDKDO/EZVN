#pragma once

#include "RAII_SDL2/renderer.h"
#include "image.h"

#include <SDL2/SDL.h>

class Background
{
	public:
		Background(const std::string_view path, sdl::Renderer& renderer);
		Background(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

		void draw(sdl::Renderer& renderer);

		std::unique_ptr<Image> image_;
		SDL_Color color_;

	protected:

	private:
};

