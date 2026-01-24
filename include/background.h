#pragma once

#include "RAII_SDL2/renderer.h"
#include "image.h"

class Background
{
	public:
		Background(std::string_view path, sdl::Renderer& renderer);
		Background(Color color);

		void draw(sdl::Renderer& renderer);
		void update();

		std::unique_ptr<Image> image_;
		Color color_;

	private:
};

