#pragma once

#include "transformstep.h"

#include <vector>
#include <unordered_map>

enum class TransformName
{
	t11,

	t21,
	t22,

	t31,
	t32,
	t33,

	t41,
	t42,
	t43,
	t44,

	f11,

	f21,
	f22,

	f31,
	f32,
	f33,

	f41,
	f42,
	f43,
	f44,
};

class Transform
{
	//TODO : on show/replace/hide => https://www.renpy.org/doc/html/transforms.html#external-atl-events
	public:
		Transform();

		struct TransformAllSteps
		{
			TransformAllSteps(/*TransformName transform_name,*/ const int number_of_transform_steps)
				: current_step_number_(0), transform_finished_(false), number_of_transform_steps_(number_of_transform_steps), transform_steps_(number_of_transform_steps)
			{}

			int current_step_number_;
			bool transform_finished_;
			int number_of_transform_steps_;
			std::vector<TransformStep> transform_steps_;
		};

		std::unordered_map<TransformName, TransformAllSteps> transforms_;

		void tcommon(const int xpos, Image& image, Transform::TransformAllSteps& transform_t) const;
		void t11(Image& image);
		void t21(Image& image);
		void t22(Image& image);
		void t31(Image& image);
		void t32(Image& image);
		void t33(Image& image);
		void t41(Image& image);
		void t42(Image& image);
		void t43(Image& image);
		void t44(Image& image);

		void focus_common(const int xpos, Image& image, Transform::TransformAllSteps& transform_f) const;
		void f11(Image& image);
		void f21(Image& image);
		void f22(Image& image);
		void f31(Image& image);
		void f32(Image& image);
		void f33(Image& image);
		void f41(Image& image);
		void f42(Image& image);
		void f43(Image& image);
		void f44(Image& image);

		void show_transform(const TransformName transform_name, Image& image);
};

