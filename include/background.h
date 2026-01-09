#pragma once

#include "RAII_SDL2/renderer.h"
#include "image.h"

#include <SDL2/SDL.h>

//TODO : enum qui contient des couleurs (ex : black, white, red etc.) ??

class Background
{
	public:
		Background(const std::string_view path, sdl::Renderer& renderer);
		Background(Color color);

		void draw(sdl::Renderer& renderer);

		std::unique_ptr<Image> image_;
		Color color_;

	protected:

	private:
};

