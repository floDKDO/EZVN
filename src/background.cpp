#include "background.h"

#include <iostream>

Background::Background(const std::string_view path, sdl::Renderer& renderer)
	: color_(Color::from_rgba8(0, 0, 0))
{
	image_ = std::make_unique<Image>(path, 0, 0, renderer, 0);
}

Background::Background(Color color)
	: color_(color)
{}

void Background::draw(sdl::Renderer& renderer)
{
	if(image_ != nullptr)
	{
		image_->draw(renderer);
	}
	else
	{
		renderer.set_draw_color(color_.r_, color_.g_, color_.b_, color_.a_);
		renderer.clear();
	}
}