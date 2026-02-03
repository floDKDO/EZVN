#pragma once

#include "image.h"
#include "duration.h"

#include <variant>

class TransformStep
{
	friend class Transform;

	public:

		struct InitialValues
		{
			InitialValues(const Image& image)
				: x_pos_(image.position_.x), y_pos_(image.position_.y), w_size_(image.position_.w), h_size_(image.position_.h),
				angle_(image.angle_), r_(image.color_.r_), g_(image.color_.g_), b_(image.color_.b_), a_(image.color_.a_)
			{}

			InitialValues()
				: x_pos_(0), y_pos_(0), w_size_(0), h_size_(0),
				angle_(0.0), r_(0), g_(0), b_(0), a_(0)
			{}

			int x_pos_;
			int y_pos_;
			int w_size_;
			int h_size_;

			double angle_;

			//TODO : utiliser Color ??
			Uint8 r_;
			Uint8 g_;
			Uint8 b_;
			Uint8 a_;
		};

		TransformStep();

		float t_;
		Uint64 start_time_;
		Uint64 elapsed_time_;
		InitialValues initial_values_;
		bool has_got_initial_values_;
		bool is_finished_;

	private:
};

namespace step
{

enum class IsPositionOffset
{
	X_OFFSET,
	Y_OFFSET,
	NONE
};

void no_modif_common(TransformStep& step, bool condition);

template<typename F>
void instant_modif_common(TransformStep& step, F instant_modif_fonc, Duration duration);

template<typename F>
void each_frame_modif_common(TransformStep& step, Image& image, F each_frame_modif_fonc, Duration duration = Duration());

void alpha_common(TransformStep& step, Uint8 alpha, Image& image, Duration duration = Duration());
void set_position_common(TransformStep& step, Image& image, int x, int y, IsPositionOffset is_offset, Duration duration = Duration());
void filter_common(TransformStep& step, Image& image, Uint8 r, Uint8 g, Uint8 b, Duration duration = Duration());

void show(TransformStep& step, Image& image, Duration duration = Duration());
void hide(TransformStep& step, Image& image, Duration duration = Duration());
void set_alpha(TransformStep& step, Image& image, Uint8 alpha, Duration duration = Duration());

void rotate(TransformStep& step, Image& image, double angle, Duration duration = Duration());

void zoom(TransformStep& step, Image& image, float zoom, Duration duration = Duration());
void resize(TransformStep& step, Image& image, int w, int h, Duration duration = Duration());

void set_position(TransformStep& step, Image& image, int x, int y, Duration duration = Duration());
void set_position_xcenter(TransformStep& step, Image& image, int x, Duration duration = Duration());
void set_position_ycenter(TransformStep& step, Image& image, int y, Duration duration = Duration());
void set_position_xycenter(TransformStep& step, Image& image, int x, int y, Duration duration = Duration());

void set_position_xoffset(TransformStep& step, Image& image, int x, Duration duration = Duration());
void set_position_yoffset(TransformStep& step, Image& image, int y, Duration duration = Duration());

void set_center(TransformStep& step, Image& image, Duration duration = Duration());

void night_filter(TransformStep& step, Image& image, Duration duration = Duration());
void afternoon_filter(TransformStep& step, Image& image, Duration duration = Duration());
void own_filter(TransformStep& step, Image& image, Uint8 r, Uint8 g, Uint8 b, Duration duration = Duration());

void reset(TransformStep& step, Image& image);

}

