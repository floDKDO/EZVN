#pragma once

#include "transformstep.h"

#include <vector>
#include <functional>
#include <unordered_map>

enum class TransformName
{
	t11,
	f11
};

class Transform
{
	//TODO : on show/replace/hide
	public:
		Transform();

		//TODO : renommer
		struct Transform_X
		{
			Transform_X(/*TransformName transform_name,*/ int number_of_transform_steps)
				: current_step_number_(0), transform_finished_(false), number_of_transform_steps_(number_of_transform_steps), transform_steps_(number_of_transform_steps)
			{}

			int current_step_number_;
			bool transform_finished_;
			int number_of_transform_steps_;
			std::vector<TransformStep> transform_steps_;
		};

		std::unordered_map<TransformName, Transform_X> transforms_;

		void t11(Image& image);
		void f11(Image& image);

		void show_transform(TransformName transform_name, Image& image);
};

