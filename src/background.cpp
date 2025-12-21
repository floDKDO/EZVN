#include "background.h"

#include <iostream>

Background::Background(const std::string_view path, sdl::Renderer& renderer)
{
	image_ = std::make_unique<Image>(path, 0, 0, renderer, 0);
}

Background::Background(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	: color_({r, g, b, a})
{}

void Background::draw(sdl::Renderer& renderer)
{
	if(image_ != nullptr)
	{
		image_->draw(renderer);
	}
	else
	{
		renderer.set_draw_color(color_.r, color_.g, color_.b, color_.a);
		renderer.clear();
	}
}