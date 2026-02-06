#include "composite_image.h"

CompositeImage::CompositeImage(std::string_view composite_image_name, int w, int h, std::initializer_list<ImageInfo> images_info)
	: composite_image_name_(composite_image_name), surface_(w, h), w_(w), h_(h)
{
	for(const ImageInfo& image_info : images_info)
	{
		sdl::Surface surface(image_info.path_);
		SDL_Rect dest = {image_info.x_, image_info.y_, 0, 0};
		surface.blit(nullptr, surface_, &dest);
	}
}