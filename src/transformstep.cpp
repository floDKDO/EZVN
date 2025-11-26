#include "transformstep.h"

#include <iostream>

//TODO : beaucoup trop de répétitions dans les méthodes de cette classe
//TODO : les cast en int()

TransformStep::TransformStep()
	: transform_step_finished_(false), is_init_(false)
{

}

void TransformStep::show(Image& image, Uint64 time)
{
	if(time == 0 && !transform_step_finished_)
	{
		image.alpha_ = 255;
		image.texture_->set_alpha_mod(image.alpha_);
		transform_step_finished_ = true;
		return;
	}

	if(!is_init_)
	{
		AlphaStep alpha_step(image.alpha_, 255);
	}

	if(!transform_step_finished_ && time != 0)
	{
		//reset(image);

		alpha_step.delta_alpha_frame_ = alpha_step.delta_alpha_ / (60.0f / (1000.0f / float(time)));
		alpha_step.f_alpha_ += alpha_step.delta_alpha_frame_;

		image.alpha_ = int(alpha_step.f_alpha_);
		image.texture_->set_alpha_mod(image.alpha_);

		if(alpha_step.initial_alpha_ < 255 && image.alpha_ >= 255)
		{
			transform_step_finished_ = true;
		}
	}
}

void TransformStep::hide(Image& image, Uint64 time)
{
	AlphaStep alpha_step(image.alpha_, 0);

	if(time == 0 && !transform_step_finished_)
	{
		image.alpha_ = 0;
		image.texture_->set_alpha_mod(image.alpha_);
		transform_step_finished_ = true;
		return;
	}

	if(!transform_step_finished_ && time != 0)
	{
		//reset(image);

		alpha_step.delta_alpha_frame_ = alpha_step.delta_alpha_ / (60.0f / (1000.0f / float(time)));
		alpha_step.f_alpha_ += alpha_step.delta_alpha_frame_;

		image.alpha_ = int(alpha_step.f_alpha_);
		image.texture_->set_alpha_mod(image.alpha_);

		if(alpha_step.initial_alpha_ > 0 && image.alpha_ <= 0)
		{
			transform_step_finished_ = true;
		}
	}
}

//TODO : utiliser double à la place de float
void TransformStep::rotate(Image& image, const double angle, Uint64 time)
{
	RotateStep rotate_step(image.angle_, angle);

	if(time == 0 && !transform_step_finished_)
	{
		image.angle_ = angle;
		transform_step_finished_ = true;
		return;
	}

	if(!transform_step_finished_ && time != 0)
	{
		//reset(image);

		rotate_step.delta_angle_frame_ = rotate_step.delta_angle_ / (60.0f / (1000.0f / float(time)));
		image.angle_ += rotate_step.delta_angle_frame_;

		if((rotate_step.initial_angle_ < angle && image.angle_ >= angle) || (rotate_step.initial_angle_ > angle && image.angle_ <= angle))
		{
			transform_step_finished_ = true;
		}
	}
}

void TransformStep::flip_vertically(Image& image)
{
	//std::cout << "FLIP V ***********************************************************\n";
	image.flip_ = SDL_FLIP_VERTICAL;
}

void TransformStep::flip_horizontally(Image& image)
{
	//std::cout << "FLIP H ***********************************************************\n";
	image.flip_ = SDL_FLIP_HORIZONTAL;
}

void TransformStep::flip_normal(Image& image)
{
	//std::cout << "FLIP N ***********************************************************\n";
	image.flip_ = SDL_FLIP_NONE;
}

int TransformStep::get_xcenter(Image& image) const
{
	//std::cout << "GET XCENTER ***********************************************************\n";
	return image.position_.w / 2;
}

int TransformStep::get_ycenter(Image& image) const
{
	//std::cout << "GET YCENTER ***********************************************************\n";
	return image.position_.h / 2;
}

void TransformStep::set_position_common(Image& image, const int x, const int y, Uint64 time)
{

}

void TransformStep::set_position(Image& image, const int x, const int y, Uint64 time)
{
	PositionStep position_step(image.position_.x, image.position_.y, x, y);

	if(time == 0 && !transform_step_finished_)
	{
		image.position_ = {x, y, image.position_.w, image.position_.h};
		transform_step_finished_ = true;
		return;
	}

	if(!transform_step_finished_ && time != 0)
	{
		//reset(image);

		position_step.delta_x_frame_ = position_step.delta_x_ / (60.0f / (1000.0f / float(time)));
		position_step.f_position_x_ += position_step.delta_x_frame_;

		position_step.delta_y_frame_ = position_step.delta_y_ / (60.0f / (1000.0f / float(time)));
		position_step.f_position_y_ += position_step.delta_y_frame_;

		image.position_.x = int(position_step.f_position_x_);
		image.position_.y = int(position_step.f_position_y_);

		if(((position_step.initial_position_x_ < x && image.position_.x >= x) || (position_step.initial_position_x_ > x && image.position_.x <= x))
		&& ((position_step.initial_position_y_ < y && image.position_.y >= y) || (position_step.initial_position_y_ > y && image.position_.y <= y)))
		{
			transform_step_finished_ = true;
		}
	}
}

void TransformStep::set_position_xcenter(Image& image, const int x, const int y, Uint64 time)
{
	PositionStep position_step(image.position_.x, image.position_.y, x - std::abs(get_xcenter(image)), y);

	if(time == 0 && !transform_step_finished_)
	{
		image.position_ = {x - std::abs(get_xcenter(image)), y, image.position_.w, image.position_.h};
		transform_step_finished_ = true;
		return;
	}

	if(!transform_step_finished_ && time != 0)
	{
		//reset(image);

		position_step.delta_x_frame_ = position_step.delta_x_ / (60.0f / (1000.0f / float(time)));
		position_step.f_position_x_ += position_step.delta_x_frame_;

		position_step.delta_y_frame_ = position_step.delta_y_ / (60.0f / (1000.0f / float(time)));
		position_step.f_position_y_ += position_step.delta_y_frame_;

		image.position_.x = int(position_step.f_position_x_);
		image.position_.y = int(position_step.f_position_y_);

		if(((position_step.initial_position_x_ < x && image.position_.x >= x) || (position_step.initial_position_x_ > x && image.position_.x <= x))
		&& ((position_step.initial_position_y_ < y && image.position_.y >= y) || (position_step.initial_position_y_ > y && image.position_.y <= y)))
		{
			transform_step_finished_ = true;
		}
	}


}

void TransformStep::set_position_ycenter(Image& image, const int x, const int y, Uint64 time)
{
	PositionStep position_step(image.position_.x, image.position_.y, x, y - std::abs(get_ycenter(image)));

	if(time == 0 && !transform_step_finished_)
	{
		image.position_ = {x, y - std::abs(get_ycenter(image)), image.position_.w, image.position_.h};
		transform_step_finished_ = true;
		return;
	}

	if(!transform_step_finished_ && time != 0)
	{
		//reset(image);

		position_step.delta_x_frame_ = position_step.delta_x_ / (60.0f / (1000.0f / float(time)));
		position_step.f_position_x_ += position_step.delta_x_frame_;

		position_step.delta_y_frame_ = position_step.delta_y_ / (60.0f / (1000.0f / float(time)));
		position_step.f_position_y_ += position_step.delta_y_frame_;

		image.position_.x = int(position_step.f_position_x_);
		image.position_.y = int(position_step.f_position_y_);

		if(((position_step.initial_position_x_ < x && image.position_.x >= x) || (position_step.initial_position_x_ > x && image.position_.x <= x))
		&& ((position_step.initial_position_y_ < y && image.position_.y >= y) || (position_step.initial_position_y_ > y && image.position_.y <= y)))
		{
			transform_step_finished_ = true;
		}
	}
}

void TransformStep::set_position_xycenter(Image& image, const int x, const int y, Uint64 time)
{
	PositionStep position_step(image.position_.x, image.position_.y, x - std::abs(get_xcenter(image)), y - std::abs(get_ycenter(image)));

	if(time == 0 && !transform_step_finished_)
	{
		image.position_ = {x - std::abs(get_xcenter(image)), y - std::abs(get_ycenter(image)), image.position_.w, image.position_.h};
		transform_step_finished_ = true;
		return;
	}

	if(!transform_step_finished_ && time != 0)
	{
		//reset(image);

		position_step.delta_x_frame_ = position_step.delta_x_ / (60.0f / (1000.0f / float(time)));
		position_step.f_position_x_ += position_step.delta_x_frame_;

		position_step.delta_y_frame_ = position_step.delta_y_ / (60.0f / (1000.0f / float(time)));
		position_step.f_position_y_ += position_step.delta_y_frame_;

		image.position_.x = int(position_step.f_position_x_);
		image.position_.y = int(position_step.f_position_y_);

		if(((position_step.initial_position_x_ < x && image.position_.x >= x) || (position_step.initial_position_x_ > x && image.position_.x <= x))
		&& ((position_step.initial_position_y_ < y && image.position_.y >= y) || (position_step.initial_position_y_ > y && image.position_.y <= y)))
		{
			transform_step_finished_ = true;
		}
	}
}

void TransformStep::zoom(Image& image, const float zoom, Uint64 time)
{
	SizeStep size_step(image.position_.w, image.position_.h, zoom); //TODO : copie...

	if(time == 0 && !transform_step_finished_)
	{
		size_step.f_size_w_ *= zoom;
		size_step.f_size_h_ *= zoom;

		image.position_.w = int(size_step.f_size_w_);
		image.position_.h = int(size_step.f_size_h_);

		transform_step_finished_ = true;

		return;
	}

	if(!transform_step_finished_ && zoom != 1.0f)
	{
		//reset(image); //TODO

		size_step.delta_w_frame_ = size_step.delta_w_ / (60.0f / (1000.0f / float(time)));
		size_step.f_size_w_ += size_step.delta_w_frame_;

		size_step.delta_h_frame_ = size_step.delta_h_ / (60.0f / (1000.0f / float(time)));
		size_step.f_size_h_ += size_step.delta_h_frame_;

		image.position_.w = int(size_step.f_size_w_);
		image.position_.h = int(size_step.f_size_h_);

		if(((zoom < 1.0f && image.position_.w <= size_step.initial_size_w_ + size_step.delta_w_) || (zoom > 1.0f && image.position_.w >= size_step.initial_size_w_ + size_step.delta_w_))
		&& ((zoom < 1.0f && image.position_.h <= size_step.initial_size_h_ + size_step.delta_h_) || (zoom > 1.0f && image.position_.h >= size_step.initial_size_h_ + size_step.delta_h_)))
		{
			transform_step_finished_ = true;
		}
	}
}

void TransformStep::resize(Image& image, const int w, const int h, Uint64 time)
{
	SizeStep size_step(image.position_.w, image.position_.h, w, h); //TODO : copie...

	if(time == 0 && !transform_step_finished_)
	{
		image.position_ = {image.position_.x, image.position_.y, w, h};
		transform_step_finished_ = true;
		return;
	}

	if(!transform_step_finished_ && time != 0)
	{
		//reset(image);

		size_step.delta_w_frame_ = size_step.delta_w_ / (60.0f / (1000.0f / float(time)));
		size_step.f_size_w_ += size_step.delta_w_frame_;

		size_step.delta_h_frame_ = size_step.delta_h_ / (60.0f / (1000.0f / float(time)));
		size_step.f_size_h_ += size_step.delta_h_frame_;

		image.position_.w = int(size_step.f_size_w_);
		image.position_.h = int(size_step.f_size_h_);

		if(((size_step.initial_size_w_ < w && image.position_.w >= w) || (size_step.initial_size_w_ > w && image.position_.w <= w))
		&& ((size_step.initial_size_h_ < h && image.position_.h >= h) || (size_step.initial_size_h_ > h && image.position_.h <= h)))
		{
			transform_step_finished_ = true;
		}
	}
}

void TransformStep::set_center(Image& image, Uint64 time)
{
	PositionStep position_step(image.position_.x, image.position_.y, 1280 / 2 - std::abs(get_xcenter(image)), 720 / 2 - std::abs(get_ycenter(image)));

	if(time == 0 && !transform_step_finished_)
	{
		image.position_ = {1280 / 2 - std::abs(get_xcenter(image)), 720 / 2 - std::abs(get_ycenter(image)), image.position_.w, image.position_.h};
		transform_step_finished_ = true;
		return;
	}

	if(!transform_step_finished_ && time != 0)
	{
		//reset(image);

		position_step.delta_x_frame_ = position_step.delta_x_ / (60.0f / (1000.0f / float(time)));
		position_step.f_position_x_ += position_step.delta_x_frame_;

		position_step.delta_y_frame_ = position_step.delta_y_ / (60.0f / (1000.0f / float(time)));
		position_step.f_position_y_ += position_step.delta_y_frame_;

		image.position_.x = int(position_step.f_position_x_);
		image.position_.y = int(position_step.f_position_y_);

		//TODO : utiliser (1280.0f / 2.0f - std::abs(get_xcenter(image))) à la place ??
		if(((position_step.initial_position_x_ < 1280 / 2 && image.position_.x >= 1280 / 2) || (position_step.initial_position_x_ > 1280 / 2 && image.position_.x <= 1280 / 2))
		&& ((position_step.initial_position_y_ < 720 / 2 && image.position_.y >= 720 / 2) || (position_step.initial_position_y_ > 720 / 2 && image.position_.y <= 720 / 2)))
		{
			transform_step_finished_ = true;
		}
	}

}

void TransformStep::night_filter(Image& image, Uint64 time)
{
	FilterStep filter_step(image.r_, image.g_, image.b_, 127, 127, 165);

	if(time == 0 && !transform_step_finished_)
	{
		image.r_ = 127;
		image.g_ = 127;
		image.b_ = 165;
		image.texture_->set_color_mod(image.r_, image.g_, image.b_);
		transform_step_finished_ = true;
		return;
	}

	if(!transform_step_finished_ && time != 0)
	{
		//reset(image);

		filter_step.delta_r_frame_ = filter_step.delta_r_ / (60.0f / (1000.0f / float(time)));
		filter_step.f_r_ += filter_step.delta_r_frame_;

		filter_step.delta_g_frame_ = filter_step.delta_g_ / (60.0f / (1000.0f / float(time)));
		filter_step.f_g_ += filter_step.delta_g_frame_;

		filter_step.delta_b_frame_ = filter_step.delta_b_ / (60.0f / (1000.0f / float(time)));
		filter_step.f_b_ += filter_step.delta_b_frame_;

		image.r_ = int(filter_step.f_r_);
		image.g_ = int(filter_step.f_g_);
		image.b_ = int(filter_step.f_b_);

		image.texture_->set_color_mod(image.r_, image.g_, image.b_);

		if(((filter_step.initial_r_ < 127 && image.r_ >= 127) || (filter_step.initial_r_ > 127 && image.r_ <= 127))
		&& ((filter_step.initial_g_ < 127 && image.g_ >= 127) || (filter_step.initial_g_ > 127 && image.g_ <= 127))
		&& ((filter_step.initial_b_ < 165 && image.b_ >= 165) || (filter_step.initial_b_ > 165 && image.b_ <= 165)))
		{
			transform_step_finished_ = true;
		}
	}
}

void TransformStep::afternoon_filter(Image& image, Uint64 time)
{
	FilterStep filter_step(image.r_, image.g_, image.b_, 210, 150, 130);

	if(time == 0 && !transform_step_finished_)
	{
		image.r_ = 210;
		image.g_ = 150;
		image.b_ = 130;
		image.texture_->set_color_mod(image.r_, image.g_, image.b_);
		transform_step_finished_ = true;
		return;
	}

	if(!transform_step_finished_ && time != 0)
	{
		//reset(image);

		filter_step.delta_r_frame_ = filter_step.delta_r_ / (60.0f / (1000.0f / float(time)));
		filter_step.f_r_ += filter_step.delta_r_frame_;

		filter_step.delta_g_frame_ = filter_step.delta_g_ / (60.0f / (1000.0f / float(time)));
		filter_step.f_g_ += filter_step.delta_g_frame_;

		filter_step.delta_b_frame_ = filter_step.delta_b_ / (60.0f / (1000.0f / float(time)));
		filter_step.f_b_ += filter_step.delta_b_frame_;

		image.r_ = int(filter_step.f_r_);
		image.g_ = int(filter_step.f_g_);
		image.b_ = int(filter_step.f_b_);

		image.texture_->set_color_mod(image.r_, image.g_, image.b_);

		if(((filter_step.initial_r_ < 210 && image.r_ >= 210) || (filter_step.initial_r_ > 210 && image.r_ <= 210))
		&& ((filter_step.initial_g_ < 150 && image.g_ >= 150) || (filter_step.initial_g_ > 150 && image.g_ <= 150))
		&& ((filter_step.initial_b_ < 130 && image.b_ >= 130) || (filter_step.initial_b_ > 130 && image.b_ <= 130)))
		{
			transform_step_finished_ = true;
		}
	}
}

void TransformStep::own_filter(Image& image, const Uint8 r, const Uint8 g, const Uint8 b, Uint64 time)
{
	FilterStep filter_step(image.r_, image.g_, image.b_, r, g, b);

	if(time == 0 && !transform_step_finished_)
	{
		image.r_ = r;
		image.g_ = g;
		image.b_ = b;
		image.texture_->set_color_mod(image.r_, image.g_, image.b_);
		transform_step_finished_ = true;
		return;
	}

	if(!transform_step_finished_ && time != 0)
	{
		//reset(image);

		filter_step.delta_r_frame_ = filter_step.delta_r_ / (60.0f / (1000.0f / float(time)));
		filter_step.f_r_ += filter_step.delta_r_frame_;

		filter_step.delta_g_frame_ = filter_step.delta_g_ / (60.0f / (1000.0f / float(time)));
		filter_step.f_g_ += filter_step.delta_g_frame_;

		filter_step.delta_b_frame_ = filter_step.delta_b_ / (60.0f / (1000.0f / float(time)));
		filter_step.f_b_ += filter_step.delta_b_frame_;

		image.r_ = int(filter_step.f_r_);
		image.g_ = int(filter_step.f_g_);
		image.b_ = int(filter_step.f_b_);

		image.texture_->set_color_mod(image.r_, image.g_, image.b_);

		if(((filter_step.initial_r_ < r && image.r_ >= r) || (filter_step.initial_r_ > r && image.r_ <= r))
		&& ((filter_step.initial_g_ < g && image.g_ >= g) || (filter_step.initial_g_ > g && image.g_ <= g))
		&& ((filter_step.initial_b_ < b && image.b_ >= b) || (filter_step.initial_b_ > b && image.b_ <= b)))
		{
			transform_step_finished_ = true;
		}
	}
}

//TODO : aussi sauvegarder angle, alpha etc ??
//TODO : si pas besoin, alors utilité d'appeler cette fonction dans les méthodes au-dessus ??
void TransformStep::reset(Image& image)
{
	image.position_ = image.initial_rect_;
}