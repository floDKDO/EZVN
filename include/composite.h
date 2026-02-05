#pragma once

#include "RAII_SDL2/surface.h"

#include <string>
#include <string_view>

struct CompositeImage
{
	std::string path_;
	int x_;
	int y_;
};

class Composite
{
	public:
		Composite(int w, int h);
		void add_image(std::string_view path, int x, int y);

		sdl::Surface surface_;

	protected:


	private:
		int w_;
		int h_;
};

