#pragma once

#include "RAII_SDL2/surface.h"

#include <string>
#include <string_view>

//TODO : remplacer par Image ??
struct ImageInfo
{
	ImageInfo(std::string_view path, int x, int y)
		: path_(path), x_(x), y_(y)
	{}

	std::string path_;
	int x_;
	int y_;
};

class CompositeImage
{
	friend class Image;

	public:
		CompositeImage(std::string_view composite_image_name, int w, int h, std::initializer_list<ImageInfo> images_info);

		std::string composite_image_name_;
		
	private:
		sdl::Surface surface_;
		int w_;
		int h_;
};

