#pragma once

#include "image.h"

#include <iostream>
#include <variant>

class TransformStep
{

	//TODO : écrire public, protected et private dans les struct => mettre les membres qui étaient const en private
	//TODO : constructeur par copie/move dans les struct ?
	//TODO : écrire les struct en dessous de la classe ?

	struct PositionStep //set_position(), set_position_xcenter(), set_position_ycenter(), set_position_xycenter(), set_center() => n'utilise pas y !
	{
		public:

			enum class SetPositionKind
			{
				SET_POSITION,
				SET_POSITION_XCENTER,
				SET_POSITION_YCENTER,
				SET_POSITION_XYCENTER,
				SET_CENTER
			};

			PositionStep(const int initial_position_x, const int initial_position_y, const int x, const int y)
				: initial_position_x_(initial_position_x), initial_position_y_(initial_position_y),
				f_position_x_(float(initial_position_x)), f_position_y_(float(initial_position_y)),
				delta_x_(x - initial_position_x), delta_y_(y - initial_position_y),
				delta_x_frame_(0.0f), delta_y_frame_(0.0f)
			{}

			int initial_position_x_;
			int initial_position_y_;

			float f_position_x_;
			float f_position_y_;

			float delta_x_;
			float delta_y_;

			float delta_x_frame_;
			float delta_y_frame_;

		private:

	};

	struct SizeStep //resize(), zoom()
	{
		//for zoom()
		SizeStep(const int initial_size_w, const int initial_size_h, const float zoom)
			: initial_size_w_(initial_size_w), initial_size_h_(initial_size_h),
			f_size_w_(float(initial_size_w)), f_size_h_(float(initial_size_h)),
			delta_w_(f_size_w_* zoom - initial_size_w), delta_h_(f_size_h_* zoom - initial_size_h),
			delta_w_frame_(0.0f), delta_h_frame_(0.0f)
		{}

		//for resize()
		SizeStep(const int initial_size_w, const int initial_size_h, const int w, const int h)
			: initial_size_w_(initial_size_w), initial_size_h_(initial_size_h),
			f_size_w_(float(initial_size_w)), f_size_h_(float(initial_size_h)),
			delta_w_(w - initial_size_w), delta_h_(h - initial_size_h),
			delta_w_frame_(0.0f), delta_h_frame_(0.0f)
		{}

		int initial_size_w_;
		int initial_size_h_;

		float f_size_w_;
		float f_size_h_;

		float delta_w_;
		float delta_h_;

		float delta_w_frame_;
		float delta_h_frame_;
	};

	struct RotateStep //rotate()
	{
		RotateStep(const double initial_angle, const double angle)
			: initial_angle_(initial_angle), delta_angle_(angle - initial_angle), delta_angle_frame_(0.0)
		{
		}

		double initial_angle_;

		double delta_angle_;
		double delta_angle_frame_;
	};

	struct AlphaStep //show(), hide(), set_alpha()
	{
		AlphaStep(const int initial_alpha, const int alpha)
			: initial_alpha_(initial_alpha), f_alpha_(float(initial_alpha)),
			delta_alpha_(alpha - initial_alpha), delta_alpha_frame_(0.0f)
		{
			std::cout << "CONSTRUCTEUR DE ALPHASTEP\n";
		}

		int initial_alpha_;

		float f_alpha_;

		float delta_alpha_;
		float delta_alpha_frame_;
	};

	struct FilterStep //night_filter(), afternoon_filter(), own_filter()
	{
		FilterStep(const int initial_r, const int initial_g, const int initial_b, const int r, const int g, const int b)
			: initial_r_(initial_r), initial_g_(initial_g), initial_b_(initial_b),
			f_r_(float(initial_r)), f_g_(float(initial_g)), f_b_(float(initial_b)),
			delta_r_(r - initial_r), delta_g_(g - initial_g), delta_b_(b - initial_b),
			delta_r_frame_(0.0f), delta_g_frame_(0.0f), delta_b_frame_(0.0f)
		{
		}

		int initial_r_;
		int initial_g_;
		int initial_b_;

		float f_r_;
		float f_g_;
		float f_b_;

		float delta_r_;
		float delta_g_;
		float delta_b_;

		float delta_r_frame_;
		float delta_g_frame_;
		float delta_b_frame_;
	};

	public:
		TransformStep();

		void flip_vertically(Image& image);
		void flip_horizontally(Image& image);
		void flip_normal(Image& image);

		int get_xcenter(Image& image) const;
		int get_ycenter(Image& image) const;

		void no_modif_common(bool condition);

		template<typename F>
		void instant_modif_common(F instant_modif_fonc);

		template<typename Factory, typename F>
		void each_frame_modif_common(Factory step_object, F each_frame_modif_fonc, Uint64 time = 0);

		void alpha_common(Uint8 alpha, Image& image, Uint64 time = 0); //TODO
		void show(Image& image, Uint64 time = 0);
		void hide(Image& image, Uint64 time = 0);
		void set_alpha(Image& image, Uint8 alpha, Uint64 time = 0);

		void rotate(Image& image, const double angle, Uint64 time = 0);

		//void size_common //TODO
		void zoom(Image& image, const float zoom, Uint64 time = 0);
		void resize(Image& image, const int w, const int h, Uint64 time = 0);

		void set_position_common(Image& image, const int x, const int y, Uint64 time = 0); //TODO
		void set_position(Image& image, const int x, const int y, Uint64 time = 0);
		void set_position_xcenter(Image& image, const int x, const int y, Uint64 time = 0);
		void set_position_ycenter(Image& image, const int x, const int y, Uint64 time = 0);
	    void set_position_xycenter(Image& image, const int x, const int y, Uint64 time = 0);
		void set_center(Image& image, Uint64 time = 0);

		void filter_common(Image& image, const Uint8 r, const Uint8 g, const Uint8 b, Uint64 time = 0); //TODO
		void night_filter(Image& image, Uint64 time = 0);
		void afternoon_filter(Image& image, Uint64 time = 0);
		void own_filter(Image& image, const Uint8 r, const Uint8 g, const Uint8 b, Uint64 time = 0);

		void reset(Image& image);

		std::variant<std::monostate, PositionStep, SizeStep, RotateStep, AlphaStep, FilterStep> current_step_; //TODO : renommer

		bool is_init_;
		bool transform_step_finished_;

	protected:

	private:

};

