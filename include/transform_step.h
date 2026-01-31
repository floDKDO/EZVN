#pragma once

#include "image.h"
#include "duration.h"

#include <variant>

class TransformStep
{
	friend class Transform;

	public:
		TransformStep();

		void show(Image& image, Duration duration = Duration());
		void hide(Image& image, Duration duration = Duration());
		void set_alpha(Image& image, Uint8 alpha, Duration duration = Duration());

		void rotate(Image& image, double angle, Duration duration = Duration());

		void zoom(Image& image, float zoom, Duration duration = Duration());
		void resize(Image& image, int w, int h, Duration duration = Duration());

		void set_position(Image& image, int x, int y, Duration duration = Duration());
		void set_position_xcenter(Image& image, int x, Duration duration = Duration());
		void set_position_ycenter(Image& image, int y, Duration duration = Duration());
	    void set_position_xycenter(Image& image, int x, int y, Duration duration = Duration());

		void set_position_xoffset(Image& image, int x, Duration duration = Duration());
		void set_position_yoffset(Image& image, int y, Duration duration = Duration());

		void set_center(Image& image, Duration duration = Duration());

		void night_filter(Image& image, Duration duration = Duration());
		void afternoon_filter(Image& image, Duration duration = Duration());
		void own_filter(Image& image, Uint8 r, Uint8 g, Uint8 b, Duration duration = Duration());

		void reset(Image& image);

		bool transform_step_finished_;

	private:
		struct Step
		{
			public:
				Step()
					: t_(0.0f), start_time_(SDL_GetTicks64()), elapsed_time_(0)
				{}

				float t_;
				Uint64 start_time_;
				Uint64 elapsed_time_;
		};

		struct PositionStep : public Step //set_position(), set_position_xcenter(), set_position_ycenter(), set_position_xycenter(), set_center() => n'utilise pas y !
		{
			public:
				PositionStep(int initial_position_x, int initial_position_y)
					: initial_position_x_(initial_position_x), initial_position_y_(initial_position_y)
				{}

				int initial_position_x_;
				int initial_position_y_;
		};

		struct SizeStep : public Step //resize(), zoom()
		{
			public: 
				SizeStep(int initial_size_w, int initial_size_h)
					: initial_size_w_(initial_size_w), initial_size_h_(initial_size_h)
				{}

				int initial_size_w_;
				int initial_size_h_;
		};

		struct RotateStep : public Step //rotate()
		{
			public:
				RotateStep(double initial_angle)
					: initial_angle_(initial_angle)
				{}

				double initial_angle_;
		};

		struct AlphaStep : public Step //show(), hide(), set_alpha()
		{
			public:
				AlphaStep(Uint8 initial_alpha)
					: initial_alpha_(initial_alpha)
				{}

				Uint8 initial_alpha_;
		};

		struct FilterStep : public Step //night_filter(), afternoon_filter(), own_filter()
		{
			public:
				FilterStep(Uint8 initial_r, Uint8 initial_g, Uint8 initial_b)
					: initial_r_(initial_r), initial_g_(initial_g), initial_b_(initial_b)
				{}

				Uint8 initial_r_;
				Uint8 initial_g_;
				Uint8 initial_b_;
		};

		enum class IsPositionOffset
		{
			X_OFFSET,
			Y_OFFSET,
			NONE
		};

		void no_modif_common(bool condition);

		template<typename F>
		void instant_modif_common(F instant_modif_fonc, Duration duration);

		template<typename Factory, typename F>
		void each_frame_modif_common(Factory step_object, F each_frame_modif_fonc, Duration duration = Duration());

		void alpha_common(Uint8 alpha, Image& image, Duration duration = Duration());
		void set_position_common(Image& image, int x, int y, IsPositionOffset is_offset, Duration duration = Duration());
		void filter_common(Image& image, Uint8 r, Uint8 g, Uint8 b, Duration duration = Duration());

		bool is_init_;
		std::variant<std::monostate, PositionStep, SizeStep, RotateStep, AlphaStep, FilterStep> step_;
};

