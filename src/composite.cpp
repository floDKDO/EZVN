#include "composite.h"

Composite::Composite(int w, int h)
	: surface_(w, h), w_(w), h_(h)
{}

void Composite::add_image(std::string_view path, int x, int y)
{
	SDL_Rect dest = {x, y, 0, 0};
	surface_.blit(nullptr, surface_, &dest);
}