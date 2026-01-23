#include "drawable.h"
#include "constants.h"

#include <iostream>

Drawable::Drawable(sdl::Renderer& renderer, Color color)
	: last_time_(0), position_({0, 0, 0, 0}), initial_rect_(position_), angle_(0.0), flip_(SDL_FLIP_NONE), color_(color), renderer_(renderer)
{}

void Drawable::show()
{
	//std::cout << "SHOW ***********************************************************\n";
	color_.a_ = 255;
	texture_->set_alpha_mod(color_.a_);
}

void Drawable::hide()
{
	//std::cout << "HIDE ***********************************************************\n";
	color_.a_ = 0;
	texture_->set_alpha_mod(color_.a_);
}

void Drawable::set_alpha(Uint8 alpha)
{
	//std::cout << "SET ALPHA ***********************************************************\n";
	color_.a_ = alpha;
	texture_->set_alpha_mod(color_.a_);
}

void Drawable::rotate(double angle)
{
	//std::cout << "ROTATE ***********************************************************\n";
	angle_ = angle;
}

void Drawable::flip_vertically()
{
	//std::cout << "FLIP V ***********************************************************\n";
	flip_ = SDL_FLIP_VERTICAL;
}

void Drawable::flip_horizontally()
{
	//std::cout << "FLIP H ***********************************************************\n";
	flip_ = SDL_FLIP_HORIZONTAL;
}

void Drawable::flip_normal()
{
	//std::cout << "FLIP N ***********************************************************\n";
	flip_ = SDL_FLIP_NONE;
}

int Drawable::get_xcenter() const
{
	//std::cout << "GET XCENTER ***********************************************************\n";
	return position_.w / 2;
}

int Drawable::get_ycenter() const
{
	//std::cout << "GET YCENTER ***********************************************************\n";
	return position_.h / 2;
}

void Drawable::zoom(float zoom)
{
	//std::cout << "ZOOM ***********************************************************\n";

	float position_w = float(position_.w);
	float position_h = float(position_.h);

	position_w *= zoom;
	position_h *= zoom;

	position_.w = int(position_w);
	position_.h = int(position_h);
}

void Drawable::resize(int w, int h)
{
	//std::cout << "RESIZE ***********************************************************\n";
	position_ = {position_.x, position_.y, w, h};
}

void Drawable::set_position(int x, int y)
{
	//std::cout << "SET POS ***********************************************************\n";
	position_ = {x, y, position_.w, position_.h};
}

void Drawable::set_position_xcenter(int x)
{
	//std::cout << "SET POS XCENTER ***********************************************************\n";
	position_ = {x - std::abs(get_xcenter()), position_.y, position_.w, position_.h};
}

void Drawable::set_position_ycenter(int y)
{
	//std::cout << "SET POS YCENTER ***********************************************************\n";
	position_ = {position_.x, y - std::abs(get_ycenter()), position_.w, position_.h};
}

void Drawable::set_position_xycenter(int x, int y)
{
	//std::cout << "SET POS XYCENTER ***********************************************************\n";
	position_ = {x - std::abs(get_xcenter()), y - std::abs(get_ycenter()), position_.w, position_.h};
}

void Drawable::set_position_xoffset(int x)
{
	//std::cout << "SET POS XOFFSET ***********************************************************\n";
	position_ = {initial_rect_.x + x, position_.y, position_.w, position_.h};
}

void Drawable::set_position_yoffset(int y)
{
	//std::cout << "SET POS YOFFSET ***********************************************************\n";
	position_ = {position_.x, initial_rect_.y + y, position_.w, position_.h};
}

void Drawable::set_center()
{
	//std::cout << "SET CENTER ***********************************************************\n";
	position_ = {constants::window_width_ / 2 - std::abs(get_xcenter()), constants::window_height_ / 2 - std::abs(get_ycenter()), position_.w, position_.h};
}

void Drawable::night_filter()
{
	//std::cout << "N FILTER ***********************************************************\n";
	color_ = Color::from_rgba8(127, 127, 165, color_.a_);
	texture_->set_color_mod(color_.r_, color_.g_, color_.b_);
}

void Drawable::afternoon_filter()
{
	//std::cout << "A FILTER ***********************************************************\n";
	color_ = Color::from_rgba8(210, 150, 130, color_.a_);
	texture_->set_color_mod(color_.r_, color_.g_, color_.b_);
}

void Drawable::change_color(Color color)
{
	//std::cout << "CHANGE COLOR ***********************************************************\n";
	color_ = color;
	texture_->set_color_mod(color_.r_, color_.g_, color_.b_);
}