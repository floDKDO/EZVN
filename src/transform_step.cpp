#include "transform_step.h"
#include "constants.h"

#include <iostream>

//TODO : paramètre qui dit la fonction qu'on va utiliser

TransformStep::TransformStep()
	: t_(0.0f), start_time_(SDL_GetTicks64()), elapsed_time_(0), is_finished_(false), has_got_initial_values_(false)
{}

float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

double lerp(double a, double b, double t)
{
	return a + t * (b - a);
}

namespace step
{

void no_modif_common(TransformStep& step, bool condition)
{
	if(condition)
	{
		step.is_finished_ = true;
		return;
	}
}

template<typename F>
void instant_modif_common(TransformStep& step, F instant_modif_fonc, Duration duration)
{
	SDL_assert(std::is_invocable_v<F>);
	if(duration.duration_ == 0 && !step.is_finished_)
	{
		instant_modif_fonc();
		step.is_finished_ = true;
		return;
	}
}

template<typename F>
void each_frame_modif_common(TransformStep& step, Image& image, F each_frame_modif_fonc, Duration duration)
{
	if(!step.is_finished_ && duration.duration_ != 0)
	{
		if(!step.has_got_initial_values_)
		{
			step.initial_values_ = TransformStep::InitialValues(image);
			step.has_got_initial_values_ = true;
		}

		step.elapsed_time_ = SDL_GetTicks64() - step.start_time_;

		float t = float(step.elapsed_time_) / float(duration.duration_);
		if(duration.kind_ == Duration::Kind::LINEAR)
		{
			step.t_ = t;
		}
		else
		{
			float epsilon = 0.01f;
			if(duration.kind_ == Duration::Kind::EASE)
			{
				step.t_ = 0.5f - std::cos(float(M_PI) * t) / 2.0f + epsilon;
			}
			else if(duration.kind_ == Duration::Kind::EASEIN)
			{
				step.t_ = std::cos((1.0f - t) * float(M_PI) / 2.0f) + epsilon;
			}
			else if(duration.kind_ == Duration::Kind::EASEOUT)
			{
				step.t_ = 1.0f - std::cos(t * float(M_PI) / 2.0f) + epsilon;
			}
		}

		if(step.t_ > 1.0f)
		{
			step.t_ = 1.0f;
			step.is_finished_ = true;
		}

		each_frame_modif_fonc();
	}
}

void alpha_common(TransformStep& step, Uint8 alpha, Image& image, Duration duration)
{
	no_modif_common(step, image.color_.a_ == alpha);

	instant_modif_common(step,
	[&]()
	{
		image.set_alpha(alpha);
	}
	, duration);

	each_frame_modif_common(step, image,
	[&]()
	{
		Uint8 new_alpha_value = Uint8(lerp(float(step.initial_values_.a_), float(alpha), step.t_));

		//std::cout << int(new_alpha_value) << ", " << int(step.initial_values_.a_) << ", " << int(alpha) << ", " << int(image.color_.a_) << std::endl;

		image.set_alpha(new_alpha_value);
	}
	, duration);
}

void show(TransformStep& step, Image& image, Duration duration)
{
	std::cout << "SHOW\n";
	std::cout << int(step.initial_values_.a_) << ", " << int(image.color_.a_) << ", " << duration.duration_ << ", " << step.t_ << std::endl;
	alpha_common(step, 255, image, duration);
}

void hide(TransformStep& step, Image& image, Duration duration)
{
	alpha_common(step, 0, image, duration);
}

void set_alpha(TransformStep& step, Image& image, Uint8 alpha, Duration duration)
{
	alpha_common(step, alpha, image, duration);
}

void rotate(TransformStep& step, Image& image, double angle, Duration duration)
{
	no_modif_common(step, image.angle_ == angle);

	instant_modif_common(step,
	[&]()
	{
		image.rotate(angle);
	}
	, duration);

	each_frame_modif_common(step, image,
	[&]()
	{
		double new_angle_value = lerp(step.initial_values_.angle_, angle, double(step.t_));
		image.rotate(new_angle_value);
	}
	, duration);
}

void set_position_common(TransformStep& step, Image& image, int x, int y, IsPositionOffset is_offset, Duration duration)
{
	no_modif_common(step, image.position_.x == x && image.position_.y == y);

	instant_modif_common(step,
	[&]()
	{
		image.set_position(x, y);
	}
	, duration);

	each_frame_modif_common(step, image,
	[&]()
	{
		if(is_offset == IsPositionOffset::X_OFFSET)
		{
			x = step.initial_values_.x_pos_ + x;
		}
		else if(is_offset == IsPositionOffset::Y_OFFSET)
		{
			y = step.initial_values_.y_pos_ + y;
		}

		int new_x_value = int(lerp(float(step.initial_values_.x_pos_), float(x), step.t_));
		int new_y_value = int(lerp(float(step.initial_values_.y_pos_), float(y), step.t_));

		image.set_position(new_x_value, new_y_value);
	}
	, duration);
}

void set_position(TransformStep& step, Image& image, int x, int y, Duration duration)
{
	set_position_common(step, image, x, y, IsPositionOffset::NONE, duration);
}

void set_position_xcenter(TransformStep& step, Image& image, int x, Duration duration)
{
	set_position_common(step, image, x - std::abs(image.get_xcenter()), image.position_.y, IsPositionOffset::NONE, duration);
}

void set_position_ycenter(TransformStep& step, Image& image, int y, Duration duration)
{
	set_position_common(step, image, image.position_.x, y - std::abs(image.get_ycenter()), IsPositionOffset::NONE, duration);
}

void set_position_xycenter(TransformStep& step, Image& image, int x, int y, Duration duration)
{
	set_position_common(step, image, x - std::abs(image.get_xcenter()), y - std::abs(image.get_ycenter()), IsPositionOffset::NONE, duration);
}

void set_position_xoffset(TransformStep& step, Image& image, int x, Duration duration)
{
	set_position_common(step, image, x, image.position_.y, IsPositionOffset::X_OFFSET, duration);
}

void set_position_yoffset(TransformStep& step, Image& image, int y, Duration duration)
{
	set_position_common(step, image, image.position_.x, y, IsPositionOffset::Y_OFFSET, duration);
}

void set_center(TransformStep& step, Image& image, Duration duration)
{
	no_modif_common(step, image.position_.x == (constants::window_width_ / 2 - std::abs(image.get_xcenter())) && image.position_.y == (constants::window_height_ / 2 - std::abs(image.get_ycenter())));

	instant_modif_common(step,
	[&]()
	{
		image.set_center();
	}
	, duration);

	each_frame_modif_common(step, image,
	[&]()
	{
		int new_x_value = int(lerp(float(step.initial_values_.x_pos_), float(constants::window_width_ / 2 - std::abs(image.get_xcenter())), step.t_));
		int new_y_value = int(lerp(float(step.initial_values_.y_pos_), float(constants::window_height_ / 2 - std::abs(image.get_ycenter())), step.t_));
		image.set_position(new_x_value, new_y_value);
	}
	, duration);
}

void zoom(TransformStep& step, Image& image, float zoom, Duration duration)
{
	no_modif_common(step, zoom == 1.0f);

	instant_modif_common(step,
	[&]()
	{
		image.zoom(zoom);
	}
	, duration);

	each_frame_modif_common(step, image,
	[&]()
	{
		int new_w_value = int(lerp(float(step.initial_values_.w_size_), float(step.initial_values_.w_size_) * zoom, step.t_));
		int new_h_value = int(lerp(float(step.initial_values_.h_size_), float(step.initial_values_.h_size_) * zoom, step.t_));
		image.resize(new_w_value, new_h_value);
	}
	, duration);
}

void resize(TransformStep& step, Image& image, int w, int h, Duration duration)
{
	no_modif_common(step, image.position_.w == w && image.position_.h == h);

	instant_modif_common(step,
	[&]()
	{
		image.resize(w, h);
	}
	, duration);

	each_frame_modif_common(step, image,
	[&]()
	{
		int new_w_value = int(lerp(float(step.initial_values_.w_size_), float(w), step.t_));
		int new_h_value = int(lerp(float(step.initial_values_.h_size_), float(h), step.t_));
		image.resize(new_w_value, new_h_value);
	}
	, duration);
}

void filter_common(TransformStep& step, Image& image, Uint8 r, Uint8 g, Uint8 b, Duration duration)
{
	no_modif_common(step, image.color_.r_ == r && image.color_.g_ == g && image.color_.b_ == b);

	instant_modif_common(step,
	[&]()
	{
		image.change_color(Color::from_rgba8(r, g, b, image.color_.a_));
	}
	, duration);

	each_frame_modif_common(step, image,
	[&]()
	{
		Uint8 new_r_value = Uint8(lerp(float(step.initial_values_.r_), float(r), step.t_));
		Uint8 new_g_value = Uint8(lerp(float(step.initial_values_.g_), float(g), step.t_));
		Uint8 new_b_value = Uint8(lerp(float(step.initial_values_.b_), float(b), step.t_));
		image.change_color(Color::from_rgba8(new_r_value, new_g_value, new_b_value));
	}
	, duration);
}

void night_filter(TransformStep& step, Image& image, Duration duration)
{
	filter_common(step, image, 127, 127, 165, duration);
}

void afternoon_filter(TransformStep& step, Image& image, Duration duration)
{
	filter_common(step, image, 210, 150, 130, duration);
}

void own_filter(TransformStep& step, Image& image, Uint8 r, Uint8 g, Uint8 b, Duration duration)
{
	filter_common(step, image, r, g, b, duration);
}

//TODO : aussi sauvegarder angle, alpha etc ??
void reset(TransformStep& step, Image& image)
{
	std::cout << "RESET\n";
	image.position_ = image.initial_rect_;
	step.is_finished_ = true;
}

}