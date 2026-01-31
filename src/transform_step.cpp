#include "transform_step.h"
#include "constants.h"

#include <iostream>

//TODO : gérer les easein/easeout 

TransformStep::TransformStep()
	: transform_step_finished_(false), is_init_(false)
{}

float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

double lerp(double a, double b, double t)
{
	return a + t * (b - a);
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
void TransformStep::instant_modif_common(F instant_modif_fonc, Uint64 duration)
{
	SDL_assert(std::is_invocable_v<F>);
	if(duration == 0 && !transform_step_finished_)
	{
		instant_modif_fonc();
		transform_step_finished_ = true;
		return;
	}
}

template<typename Factory, typename F> 
void TransformStep::each_frame_modif_common(Factory step_object, F each_frame_modif_fonc, Uint64 duration)
{
	SDL_assert(std::is_invocable_v<Factory>);
	if(!transform_step_finished_ && duration != 0)
	{
		if(!is_init_)
		{
			step_ = step_object();
			is_init_ = true;
		}

		using step_t = std::decay_t<decltype(step_object())>; //get the type by removing first reference and const/volatile qualifiers to be compatible with the types of the std::variant step_
		step_t& current_step = std::get<step_t>(step_);

		current_step.elapsed_time_ = SDL_GetTicks64() - current_step.start_time_;
		current_step.t_ = float(current_step.elapsed_time_) / float(duration);

		if(current_step.t_ > 1.0f)
		{
			current_step.t_ = 1.0f;
			transform_step_finished_ = true;
		}

		each_frame_modif_fonc(current_step); 
	}
}

void TransformStep::alpha_common(Uint8 alpha, Image& image, Uint64 duration)
{
	no_modif_common(image.color_.a_ == alpha);

	instant_modif_common([&]()
	{
		image.set_alpha(alpha);
	}
	, duration);

	each_frame_modif_common([&](){ return AlphaStep(image.color_.a_); },
	[&](AlphaStep& alpha_step)
	{
		Uint8 new_alpha_value = Uint8(lerp(float(alpha_step.initial_alpha_), float(alpha), alpha_step.t_));
		image.set_alpha(new_alpha_value);
	}
	, duration);
}

void TransformStep::show(Image& image, Uint64 duration)
{
	alpha_common(255, image, duration);
}

void TransformStep::hide(Image& image, Uint64 duration)
{
	alpha_common(0, image, duration);
}

void TransformStep::set_alpha(Image& image, Uint8 alpha, Uint64 duration)
{
	alpha_common(alpha, image, duration);
}

void TransformStep::rotate(Image& image, double angle, Uint64 duration)
{
	no_modif_common(image.angle_ == angle);

	instant_modif_common([&]()
	{
		image.rotate(angle);
	}
	, duration);

	each_frame_modif_common([&](){ return RotateStep(image.angle_); },
	[&](RotateStep& rotate_step)
	{
		double new_angle_value = lerp(rotate_step.initial_angle_, angle, double(rotate_step.t_));
		image.rotate(new_angle_value);
	}
	, duration);
}

void TransformStep::set_position_common(Image& image, int x, int y, Uint64 duration)
{
	no_modif_common(image.position_.x == x && image.position_.y == y);

	instant_modif_common([&]()
	{
		image.set_position(x, y);
	}
	, duration);

	each_frame_modif_common([&](){ return PositionStep(image.position_.x, image.position_.y); },
	[&](PositionStep& position_step)
	{
		int new_x_value = int(lerp(float(position_step.initial_position_x_), float(x), position_step.t_));
		int new_y_value = int(lerp(float(position_step.initial_position_y_), float(y), position_step.t_));
		image.set_position(new_x_value, new_y_value);
	}
	, duration);
}

void TransformStep::set_position(Image& image, int x, int y, Uint64 duration)
{
	set_position_common(image, x, y, duration);
}

void TransformStep::set_position_xcenter(Image& image, int x, Uint64 duration)
{
	set_position_common(image, x - std::abs(image.get_xcenter()), image.position_.y, duration);
}

void TransformStep::set_position_ycenter(Image& image, int y, Uint64 duration)
{
	set_position_common(image, image.position_.x, y - std::abs(image.get_ycenter()), duration);
}

void TransformStep::set_position_xycenter(Image& image, int x, int y, Uint64 duration)
{
	set_position_common(image, x - std::abs(image.get_xcenter()), y - std::abs(image.get_ycenter()), duration);
}

void TransformStep::set_position_xoffset(Image& image, int x, Uint64 duration)
{
	set_position_common(image, image.initial_rect_.x + x, image.position_.y, duration);
}

void TransformStep::set_position_yoffset(Image& image, int y, Uint64 duration)
{
	set_position_common(image, image.position_.x, image.initial_rect_.y + y, duration);
}

void TransformStep::set_center(Image& image, Uint64 duration)
{
	no_modif_common(image.position_.x == (constants::window_width_ / 2 - std::abs(image.get_xcenter())) && image.position_.y == (constants::window_height_ / 2 - std::abs(image.get_ycenter())));

	instant_modif_common([&]()
	{
		image.set_center();
	}
	, duration);

	each_frame_modif_common([&](){ return PositionStep(image.position_.x, image.position_.y); },
	[&](PositionStep& position_step) 
	{
		int new_x_value = int(lerp(float(position_step.initial_position_x_), float(constants::window_width_ / 2 - std::abs(image.get_xcenter())), position_step.t_));
		int new_y_value = int(lerp(float(position_step.initial_position_y_), float(constants::window_height_ / 2 - std::abs(image.get_ycenter())), position_step.t_));
		image.set_position(new_x_value, new_y_value);
	}
	, duration);
}

void TransformStep::zoom(Image& image, float zoom, Uint64 duration)
{
	no_modif_common(zoom == 1.0f);

	instant_modif_common([&]()
	{
		image.zoom(zoom);
	}
	, duration);

	each_frame_modif_common([&](){ return SizeStep(image.position_.w, image.position_.h); },
	[&](SizeStep& size_step)
	{
		int new_w_value = int(lerp(float(size_step.initial_size_w_), float(size_step.initial_size_w_) * zoom, size_step.t_));
		int new_h_value = int(lerp(float(size_step.initial_size_h_), float(size_step.initial_size_h_) * zoom, size_step.t_));
		image.resize(new_w_value, new_h_value);
	}
	, duration);
}

void TransformStep::resize(Image& image, int w, int h, Uint64 duration)
{
	no_modif_common(image.position_.w == w && image.position_.h == h);

	instant_modif_common([&]()
	{
		image.resize(w, h);
	}
	, duration);

	each_frame_modif_common([&](){ return SizeStep(image.position_.w, image.position_.h); },
	[&](SizeStep& size_step)
	{
		int new_w_value = int(lerp(float(size_step.initial_size_w_), float(w), size_step.t_));
		int new_h_value = int(lerp(float(size_step.initial_size_h_), float(h), size_step.t_));
		image.resize(new_w_value, new_h_value);
	}
	, duration);
}

void TransformStep::filter_common(Image& image, Uint8 r, Uint8 g, Uint8 b, Uint64 duration)
{
	no_modif_common(image.color_.r_ == r && image.color_.g_ == g && image.color_.b_ == b);

	instant_modif_common([&]()
	{
		image.change_color(Color::from_rgba8(r, g, b, image.color_.a_));
	}
	, duration);

	each_frame_modif_common([&](){ return FilterStep(image.color_.r_, image.color_.g_, image.color_.b_); },
	[&](FilterStep& filter_step)
	{
		Uint8 new_r_value = Uint8(lerp(float(filter_step.initial_r_), float(r), filter_step.t_));
		Uint8 new_g_value = Uint8(lerp(float(filter_step.initial_g_), float(g), filter_step.t_));
		Uint8 new_b_value = Uint8(lerp(float(filter_step.initial_b_), float(b), filter_step.t_));
		image.change_color(Color::from_rgba8(new_r_value, new_g_value, new_b_value));
	}
	, duration);
}

void TransformStep::night_filter(Image& image, Uint64 duration)
{
	filter_common(image, 127, 127, 165, duration);
}

void TransformStep::afternoon_filter(Image& image, Uint64 duration)
{
	filter_common(image, 210, 150, 130, duration);
}

void TransformStep::own_filter(Image& image, Uint8 r, Uint8 g, Uint8 b, Uint64 duration)
{
	filter_common(image, r, g, b, duration);
}

//TODO : aussi sauvegarder angle, alpha etc ??
void TransformStep::reset(Image& image) 
{
	image.position_ = image.initial_rect_;
	transform_step_finished_ = true;
}