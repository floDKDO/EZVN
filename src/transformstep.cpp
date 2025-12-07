#include "transformstep.h"
#include "constants.h"

#include <iostream>

//TODO : bug si plusieurs transformation d'une même step modifient les mêmes coordonnées (ex : zoom et set_position) ??
//TODO : si zoom et set_position à la même step => fonctionne si zoom avant set_position mais pas si zoom est après set_position !!

TransformStep::TransformStep()
	: transform_step_finished_(false), is_init_(false)
{}

void TransformStep::no_modif_common(const bool condition)
{
	if(condition)
	{
		transform_step_finished_ = true;
		return;
	}
}

template<typename F>
void TransformStep::instant_modif_common(F instant_modif_fonc, const Uint64 time)
{
	if(time == 0 && !transform_step_finished_)
	{
		instant_modif_fonc();
		transform_step_finished_ = true;
		return;
	}
}

template<typename Factory, typename F> 
void TransformStep::each_frame_modif_common(Factory step_object, F each_frame_modif_fonc, const Uint64 time)
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

void TransformStep::alpha_common(Uint8 alpha, Image& image, const Uint64 time)
{
	no_modif_common(image.color_.a == alpha);

	instant_modif_common([&]()
	{
		image.set_alpha(alpha);
	}
	, time);

	each_frame_modif_common([&](){ return AlphaStep(image.color_.a, alpha); },
	[&](AlphaStep& alpha_step) -> bool
	{
		alpha_step.delta_alpha_frame_ = alpha_step.delta_alpha_ / (60.0f / (1000.0f / float(time)));
		alpha_step.f_alpha_ += alpha_step.delta_alpha_frame_;

		image.color_.a = Uint8(alpha_step.f_alpha_);
		image.texture_->set_alpha_mod(image.color_.a);

		return (alpha_step.initial_alpha_ > alpha && image.color_.a <= alpha) || (alpha_step.initial_alpha_ < alpha && image.color_.a >= alpha);
	}
	, time);
}

void TransformStep::show(Image& image, const Uint64 time)
{
	alpha_common(255, image, time);
}

void TransformStep::hide(Image& image, const Uint64 time)
{
	alpha_common(0, image, time);
}

void TransformStep::set_alpha(Image& image, const  Uint8 alpha, const Uint64 time)
{
	alpha_common(alpha, image, time);
}

void TransformStep::rotate(Image& image, const double angle, const Uint64 time)
{
	no_modif_common(image.angle_ == angle);

	instant_modif_common([&]()
	{
		image.rotate(angle);
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

void TransformStep::set_position_common(Image& image, const int x, const int y, const Uint64 time)
{
	no_modif_common(image.position_.x == x && image.position_.y == y);

	instant_modif_common([&]()
	{
		image.set_position(x, y);
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

void TransformStep::set_position(Image& image, const int x, const int y, const Uint64 time)
{
	set_position_common(image, x, y, time);
}

void TransformStep::set_position_xcenter(Image& image, const int x, const Uint64 time)
{
	set_position_common(image, x - std::abs(image.get_xcenter()), image.position_.y, time);
}

void TransformStep::set_position_ycenter(Image& image, const int y, const Uint64 time)
{
	set_position_common(image, image.position_.x, y - std::abs(image.get_ycenter()), time);
}

void TransformStep::set_position_xycenter(Image& image, const int x, const int y, const Uint64 time)
{
	set_position_common(image, x - std::abs(image.get_xcenter()), y - std::abs(image.get_ycenter()), time); 
}

void TransformStep::set_position_xoffset(Image& image, const int x, const Uint64 time)
{
	set_position_common(image, image.initial_rect_.x + x, image.position_.y, time);
}

void TransformStep::set_position_yoffset(Image& image, const int y, const Uint64 time)
{
	set_position_common(image, image.position_.x, image.initial_rect_.y + y, time);
}

void TransformStep::set_center(Image& image, const Uint64 time)
{
	no_modif_common(image.position_.x == (constants::window_width_ / 2 - std::abs(image.get_xcenter())) && image.position_.y == (constants::window_height_ / 2 - std::abs(image.get_ycenter())));

	instant_modif_common([&]()
	{
		image.set_center();
	}
	, time);

	each_frame_modif_common([&](){ return PositionStep(image.position_.x, image.position_.y, constants::window_width_ / 2 - std::abs(image.get_xcenter()), constants::window_height_ / 2 - std::abs(image.get_ycenter())); },
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

		return ((position_step.initial_position_x_ <= constants::window_width_ / 2 - std::abs(image.get_xcenter()) && image.position_.x >= constants::window_width_ / 2 - std::abs(image.get_xcenter())) 
			|| (position_step.initial_position_x_ > constants::window_width_ / 2 - std::abs(image.get_xcenter()) && image.position_.x <= constants::window_width_ / 2 - std::abs(image.get_xcenter())))
			&& ((position_step.initial_position_y_ <= constants::window_height_ / 2 - std::abs(image.get_ycenter()) && image.position_.y >= constants::window_height_ / 2 - std::abs(image.get_ycenter())) 
			|| (position_step.initial_position_y_ > constants::window_height_ / 2 - std::abs(image.get_ycenter()) && image.position_.y <= constants::window_height_ / 2 - std::abs(image.get_ycenter())));
	}
	, time);
}

void TransformStep::zoom(Image& image, const float zoom, const Uint64 time)
{
	no_modif_common(zoom == 1.0f);

	instant_modif_common([&]()
	{
		image.zoom(zoom);
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

void TransformStep::resize(Image& image, const int w, const int h, const Uint64 time)
{
	no_modif_common(image.position_.w == w && image.position_.h == h);

	instant_modif_common([&]()
	{
		image.resize(w, h);
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

void TransformStep::filter_common(Image& image, const Uint8 r, const Uint8 g, const Uint8 b, const Uint64 time)
{
	no_modif_common(image.color_.r == r && image.color_.g == g && image.color_.b == b);

	instant_modif_common([&]()
	{
		image.change_color({r, g, b, image.color_.a});
	}
	, time);

	each_frame_modif_common([&](){ return FilterStep(image.color_.r, image.color_.g, image.color_.b, r, g, b); },
	[&](FilterStep& filter_step) -> bool
	{
		filter_step.delta_r_frame_ = filter_step.delta_r_ / (60.0f / (1000.0f / float(time)));
		filter_step.f_r_ += filter_step.delta_r_frame_;

		filter_step.delta_g_frame_ = filter_step.delta_g_ / (60.0f / (1000.0f / float(time)));
		filter_step.f_g_ += filter_step.delta_g_frame_;

		filter_step.delta_b_frame_ = filter_step.delta_b_ / (60.0f / (1000.0f / float(time)));
		filter_step.f_b_ += filter_step.delta_b_frame_;

		image.color_.r = Uint8(filter_step.f_r_);
		image.color_.g = Uint8(filter_step.f_g_);
		image.color_.b = Uint8(filter_step.f_b_);

		image.texture_->set_color_mod(image.color_.r, image.color_.g, image.color_.b);

		return ((filter_step.initial_r_ <= r && image.color_.r >= r) || (filter_step.initial_r_ > r && image.color_.r <= r))
			&& ((filter_step.initial_g_ <= g && image.color_.g >= g) || (filter_step.initial_g_ > g && image.color_.g <= g))
			&& ((filter_step.initial_b_ <= b && image.color_.b >= b) || (filter_step.initial_b_ > b && image.color_.b <= b));
	}
	, time);
}

void TransformStep::night_filter(Image& image, const Uint64 time)
{
	filter_common(image, 127, 127, 165, time);
}

void TransformStep::afternoon_filter(Image& image, const Uint64 time)
{
	filter_common(image, 210, 150, 130, time);
}

void TransformStep::own_filter(Image& image, const Uint8 r, const Uint8 g, const Uint8 b, const Uint64 time)
{
	filter_common(image, r, g, b, time);
}

//TODO : aussi sauvegarder angle, alpha etc ??
void TransformStep::reset(Image& image) 
{
	image.position_ = image.initial_rect_;
	transform_step_finished_ = true;
}