#include "transformstep.h"
#include "constants.h"

#include <iostream>

//TODO : bug si plusieurs transformation d'une même step modifient les mêmes coordonnées (ex : zoom et set_position) ??
//TODO : si zoom et set_position à la même step => fonctionne si zoom avant set_position mais pas si zoom est après set_position !!

TransformStep::TransformStep()
	: transform_step_finished_(false), is_init_(false)
{

}

void TransformStep::flip_vertically(Image& image)
{
	image.flip_ = SDL_FLIP_VERTICAL;
}

void TransformStep::flip_horizontally(Image& image)
{
	image.flip_ = SDL_FLIP_HORIZONTAL;
}

void TransformStep::flip_normal(Image& image)
{
	image.flip_ = SDL_FLIP_NONE;
}

int TransformStep::get_xcenter(Image& image) const
{
	return image.position_.w / 2;
}

int TransformStep::get_ycenter(Image& image) const
{
	return image.position_.h / 2;
}

void TransformStep::no_modif_common(bool condition)
{
	if(condition)
	{
		transform_step_finished_ = true;
		return;
	}
}

template<typename F>
void TransformStep::instant_modif_common(F instant_modif_fonc, Uint64 time)
{
	if(time == 0 && !transform_step_finished_)
	{
		instant_modif_fonc();
		transform_step_finished_ = true;
		return;
	}
}

template<typename Factory, typename F> 
void TransformStep::each_frame_modif_common(Factory step_object, F each_frame_modif_fonc, Uint64 time)
{
	if(!transform_step_finished_ && time != 0)
	{
		//reset(image);

		if(!is_init_)
		{
			step_ = step_object();
			is_init_ = true;
		}

		using step_t = std::decay_t<decltype(step_object())>; //get the type by removing first reference and const/volatile qualifiers to be compatible with the types of the std::variant step_
		step_t& t = std::get<step_t>(step_);

		if(each_frame_modif_fonc(t)) 
		{
			std::cout << "FINISHED!\n";
			transform_step_finished_ = true;
		}
	}
}

void TransformStep::alpha_common(Uint8 alpha, Image& image, Uint64 time)
{
	no_modif_common(image.alpha_ == alpha);

	instant_modif_common([&]()
	{
		image.alpha_ = alpha;
		image.texture_->set_alpha_mod(image.alpha_);
	}
	, time);

	each_frame_modif_common([&](){ return AlphaStep(image.alpha_, alpha); },
	[&](AlphaStep& alpha_step) -> bool
	{
		alpha_step.delta_alpha_frame_ = alpha_step.delta_alpha_ / (60.0f / (1000.0f / float(time)));
		alpha_step.f_alpha_ += alpha_step.delta_alpha_frame_;

		image.alpha_ = Uint8(alpha_step.f_alpha_);
		image.texture_->set_alpha_mod(image.alpha_);

		return (alpha_step.initial_alpha_ > alpha && image.alpha_ <= alpha) || (alpha_step.initial_alpha_ < alpha && image.alpha_ >= alpha);
	}
	, time);
}

void TransformStep::show(Image& image, Uint64 time)
{
	alpha_common(255, image, time);
}

void TransformStep::hide(Image& image, Uint64 time)
{
	alpha_common(0, image, time);
}

void TransformStep::set_alpha(Image& image, Uint8 alpha, Uint64 time)
{
	alpha_common(alpha, image, time);
}

void TransformStep::rotate(Image& image, const double angle, Uint64 time)
{
	no_modif_common(image.angle_ == angle);

	instant_modif_common([&]()
	{
		image.angle_ = angle;
	}
	, time);

	each_frame_modif_common([&](){ return RotateStep(image.angle_, angle); },
	[&](RotateStep& rotate_step) -> bool
	{
		rotate_step.delta_angle_frame_ = rotate_step.delta_angle_ / (60.0f / (1000.0f / double(time)));
		image.angle_ += rotate_step.delta_angle_frame_;

		return (rotate_step.initial_angle_ < angle && image.angle_ >= angle) || (rotate_step.initial_angle_ > angle && image.angle_ <= angle);
	}
	, time);
}

void TransformStep::set_position_common(Image& image, const int x, const int y, Uint64 time)
{
	no_modif_common(image.position_.x == x && image.position_.y == y);

	instant_modif_common([&]()
	{
		image.position_ = {x, y, image.position_.w, image.position_.h};
	}
	, time);

	each_frame_modif_common([&](){ return PositionStep(image.position_.x, image.position_.y, x, y); },
	[&](PositionStep& position_step) -> bool
	{
		position_step.delta_x_frame_ = position_step.delta_x_ / (60.0f / (1000.0f / float(time)));
		position_step.f_position_x_ += position_step.delta_x_frame_;

		position_step.delta_y_frame_ = position_step.delta_y_ / (60.0f / (1000.0f / float(time)));
		position_step.f_position_y_ += position_step.delta_y_frame_;

		image.position_.x = int(position_step.f_position_x_);
		image.position_.y = int(position_step.f_position_y_);

		//std::cout << "x: " << x << " et initial_x : " << position_step.initial_position_x_ << " et image_x : " << image.position_.x << std::endl;
		//std::cout << "y: " << y << " et initial_y: " << position_step.initial_position_y_ << " et image_y: " << image.position_.y << std::endl;
		//std::cout << "\n";

		return ((position_step.initial_position_x_ <= x && image.position_.x >= x) || (position_step.initial_position_x_ > x && image.position_.x <= x))
			&& ((position_step.initial_position_y_ <= y && image.position_.y >= y) || (position_step.initial_position_y_ > y && image.position_.y <= y));
	}
	, time);
}

void TransformStep::set_position(Image& image, const int x, const int y, Uint64 time)
{
	set_position_common(image, x, y, time);
}

void TransformStep::set_position_xcenter(Image& image, const int x, Uint64 time)
{
	set_position_common(image, x - std::abs(get_xcenter(image)), image.position_.y, time);
}

void TransformStep::set_position_ycenter(Image& image, const int y, Uint64 time)
{
	set_position_common(image, image.position_.x, y - std::abs(get_ycenter(image)), time);
}

void TransformStep::set_position_xycenter(Image& image, const int x, const int y, Uint64 time)
{
	set_position_common(image, x - std::abs(get_xcenter(image)), y - std::abs(get_ycenter(image)), time); 
}

void TransformStep::set_position_xoffset(Image& image, const int x, Uint64 time)
{
	set_position_common(image, image.position_.x + x, image.position_.y, time);
}

void TransformStep::set_position_yoffset(Image& image, const int y, Uint64 time)
{
	set_position_common(image, image.position_.x, image.position_.y + y, time);
}

void TransformStep::set_center(Image& image, Uint64 time)
{
	no_modif_common(image.position_.x == (constants::window_width_ / 2 - std::abs(get_xcenter(image))) && image.position_.y == (constants::window_height_ / 2 - std::abs(get_ycenter(image))));

	instant_modif_common([&]()
	{
		image.position_ = {constants::window_width_ / 2 - std::abs(get_xcenter(image)), constants::window_height_ / 2 - std::abs(get_ycenter(image)), image.position_.w, image.position_.h};
	}
	, time);

	each_frame_modif_common([&](){ return PositionStep(image.position_.x, image.position_.y, constants::window_width_ / 2 - std::abs(get_xcenter(image)), constants::window_height_ / 2 - std::abs(get_ycenter(image))); },
	[&](PositionStep& position_step) -> bool
	{
		position_step.delta_x_frame_ = position_step.delta_x_ / (60.0f / (1000.0f / float(time)));
		position_step.f_position_x_ += position_step.delta_x_frame_;

		position_step.delta_y_frame_ = position_step.delta_y_ / (60.0f / (1000.0f / float(time)));
		position_step.f_position_y_ += position_step.delta_y_frame_;

		image.position_.x = int(position_step.f_position_x_);
		image.position_.y = int(position_step.f_position_y_);

		//std::cout << "x: " << constants::window_width_ / 2 - std::abs(get_xcenter(image)) << " et initial_x : " << position_step.initial_position_x_ << " et image_x : " << image.position_.x << std::endl;
		//std::cout << "y: " << constants::window_height_ / 2  - std::abs(get_ycenter(image)) << " et initial_y: " << position_step.initial_position_y_ << " et image_y: " << image.position_.y << std::endl;
		//std::cout << "\n";

		return ((position_step.initial_position_x_ <= constants::window_width_ / 2 - std::abs(get_xcenter(image)) && image.position_.x >= constants::window_width_ / 2 - std::abs(get_xcenter(image))) 
			|| (position_step.initial_position_x_ > constants::window_width_ / 2 - std::abs(get_xcenter(image)) && image.position_.x <= constants::window_width_ / 2 - std::abs(get_xcenter(image))))
			&& ((position_step.initial_position_y_ <= constants::window_height_ / 2 - std::abs(get_ycenter(image)) && image.position_.y >= constants::window_height_ / 2 - std::abs(get_ycenter(image))) 
			|| (position_step.initial_position_y_ > constants::window_height_ / 2 - std::abs(get_ycenter(image)) && image.position_.y <= constants::window_height_ / 2 - std::abs(get_ycenter(image))));
	}
	, time);
}

void TransformStep::zoom(Image& image, const float zoom, Uint64 time)
{
	no_modif_common(zoom == 1.0f);

	instant_modif_common([&]()
	{
		float f_size_w = float(image.position_.w);
		float f_size_h = float(image.position_.h);

		f_size_w *= zoom;
		f_size_h *= zoom;

		image.position_.w = int(f_size_w);
		image.position_.h = int(f_size_h);
	}
	, time);

	each_frame_modif_common([&](){ return SizeStep(image.position_.w, image.position_.h, zoom); },
	[&](SizeStep& size_step) -> bool
	{
		size_step.delta_w_frame_ = size_step.delta_w_ / (60.0f / (1000.0f / float(time)));
		size_step.f_size_w_ += size_step.delta_w_frame_;

		size_step.delta_h_frame_ = size_step.delta_h_ / (60.0f / (1000.0f / float(time)));
		size_step.f_size_h_ += size_step.delta_h_frame_;

		image.position_.w = int(size_step.f_size_w_);
		image.position_.h = int(size_step.f_size_h_);

		return ((zoom <= 1.0f && image.position_.w <= size_step.initial_size_w_ + size_step.delta_w_) || (zoom > 1.0f && image.position_.w >= size_step.initial_size_w_ + size_step.delta_w_))
			&& ((zoom <= 1.0f && image.position_.h <= size_step.initial_size_h_ + size_step.delta_h_) || (zoom > 1.0f && image.position_.h >= size_step.initial_size_h_ + size_step.delta_h_));
	}
	, time);
}

void TransformStep::resize(Image& image, const int w, const int h, Uint64 time)
{
	no_modif_common(image.position_.w == w && image.position_.h == h);

	instant_modif_common([&]()
	{
		image.position_ = {image.position_.x, image.position_.y, w, h};
	}
	, time);

	each_frame_modif_common([&](){ return SizeStep(image.position_.w, image.position_.h, w, h); },
	[&](SizeStep& size_step) -> bool
	{
		size_step.delta_w_frame_ = size_step.delta_w_ / (60.0f / (1000.0f / float(time)));
		size_step.f_size_w_ += size_step.delta_w_frame_;

		size_step.delta_h_frame_ = size_step.delta_h_ / (60.0f / (1000.0f / float(time)));
		size_step.f_size_h_ += size_step.delta_h_frame_;

		image.position_.w = int(size_step.f_size_w_);
		image.position_.h = int(size_step.f_size_h_);

		return ((size_step.initial_size_w_ <= w && image.position_.w >= w) || (size_step.initial_size_w_ > w && image.position_.w <= w))
			&& ((size_step.initial_size_h_ <= h && image.position_.h >= h) || (size_step.initial_size_h_ > h && image.position_.h <= h));
	}
	, time);
}

void TransformStep::filter_common(Image& image, const Uint8 r, const Uint8 g, const Uint8 b, Uint64 time)
{
	no_modif_common(image.r_ == r && image.g_ == g && image.b_ == b);

	instant_modif_common([&]()
	{
		image.r_ = r;
		image.g_ = g;
		image.b_ = b;
		image.texture_->set_color_mod(image.r_, image.g_, image.b_);
	}
	, time);

	each_frame_modif_common([&](){ return FilterStep(image.r_, image.g_, image.b_, r, g, b); },
	[&](FilterStep& filter_step) -> bool
	{
		filter_step.delta_r_frame_ = filter_step.delta_r_ / (60.0f / (1000.0f / float(time)));
		filter_step.f_r_ += filter_step.delta_r_frame_;

		filter_step.delta_g_frame_ = filter_step.delta_g_ / (60.0f / (1000.0f / float(time)));
		filter_step.f_g_ += filter_step.delta_g_frame_;

		filter_step.delta_b_frame_ = filter_step.delta_b_ / (60.0f / (1000.0f / float(time)));
		filter_step.f_b_ += filter_step.delta_b_frame_;

		image.r_ = Uint8(filter_step.f_r_);
		image.g_ = Uint8(filter_step.f_g_);
		image.b_ = Uint8(filter_step.f_b_);

		image.texture_->set_color_mod(image.r_, image.g_, image.b_);

		return ((filter_step.initial_r_ <= r && image.r_ >= r) || (filter_step.initial_r_ > r && image.r_ <= r))
			&& ((filter_step.initial_g_ <= g && image.g_ >= g) || (filter_step.initial_g_ > g && image.g_ <= g))
			&& ((filter_step.initial_b_ <= b && image.b_ >= b) || (filter_step.initial_b_ > b && image.b_ <= b));
	}
	, time);
}

void TransformStep::night_filter(Image& image, Uint64 time)
{
	filter_common(image, 127, 127, 165, time);
}

void TransformStep::afternoon_filter(Image& image, Uint64 time)
{
	filter_common(image, 210, 150, 130, time);
}

void TransformStep::own_filter(Image& image, const Uint8 r, const Uint8 g, const Uint8 b, Uint64 time)
{
	filter_common(image, r, g, b, time);
}

//TODO : aussi sauvegarder angle, alpha etc ??
//TODO : si pas besoin, alors utilité d'appeler cette fonction dans les méthodes au-dessus ??
void TransformStep::reset(Image& image)
{
	image.position_ = image.initial_rect_;
}