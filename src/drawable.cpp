#include "drawable.h"
#include "constants.h"

Drawable::Drawable()
	: last_time_(0), position_({0, 0, 0, 0}), initial_rect_(position_), angle_(0.0),
	flip_(SDL_FLIP_NONE), color_({255, 255, 255, 255})
{}

void Drawable::show()
{
	//std::cout << "SHOW ***********************************************************\n";
	color_.a = 255;
	texture_->set_alpha_mod(color_.a);
}

void Drawable::hide()
{
	//std::cout << "HIDE ***********************************************************\n";
	color_.a = 0;
	texture_->set_alpha_mod(color_.a);
}

void Drawable::set_alpha(const Uint8 alpha)
{
	//std::cout << "SET ALPHA ***********************************************************\n";
	color_.a = alpha;
	texture_->set_alpha_mod(color_.a);
}

void Drawable::rotate(const double angle)
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

void Drawable::zoom(const float zoom)
{
	//std::cout << "ZOOM ***********************************************************\n";

	float position_w = float(position_.w);
	float position_h = float(position_.h);

	position_w *= zoom;
	position_h *= zoom;

	position_.w = int(position_w);
	position_.h = int(position_h);
}

void Drawable::resize(const int w, const int h)
{
	//std::cout << "RESIZE ***********************************************************\n";
	position_ = {position_.x, position_.y, w, h};
}

void Drawable::set_position(const int x, const int y)
{
	//std::cout << "SET POS ***********************************************************\n";
	position_ = {x, y, position_.w, position_.h};
}

void Drawable::set_position_xcenter(const int x)
{
	//std::cout << "SET POS XCENTER ***********************************************************\n";
	position_ = {x - std::abs(get_xcenter()), position_.y, position_.w, position_.h};
}

void Drawable::set_position_ycenter(const int y)
{
	//std::cout << "SET POS YCENTER ***********************************************************\n";
	position_ = {position_.x, y - std::abs(get_ycenter()), position_.w, position_.h};
}

void Drawable::set_position_xycenter(const int x, const int y)
{
	//std::cout << "SET POS XYCENTER ***********************************************************\n";
	position_ = {x - std::abs(get_xcenter()), y - std::abs(get_ycenter()), position_.w, position_.h};
}

void Drawable::set_position_xoffset(const int x)
{
	//std::cout << "SET POS XOFFSET ***********************************************************\n";
	position_ = {initial_rect_.x + x, position_.y, position_.w, position_.h};
}

void Drawable::set_position_yoffset(const int y)
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
	color_ = {127, 127, 165, color_.a};
	texture_->set_color_mod(color_.r, color_.g, color_.b);
}

void Drawable::afternoon_filter()
{
	//std::cout << "A FILTER ***********************************************************\n";
	color_ = {210, 150, 130, color_.a};
	texture_->set_color_mod(color_.r, color_.g, color_.b);
}

void Drawable::change_color(const SDL_Color color)
{
	//std::cout << "CHANGE COLOR ***********************************************************\n";
	color_ = color;
	texture_->set_color_mod(color_.r, color_.g, color_.b);
}