#pragma once

#include "RAII_SDL2/renderer.h"

class Ui
{
	public:
		virtual ~Ui() = default;
		virtual void draw(sdl::Renderer& renderer) = 0;
		virtual void update() = 0;
};

