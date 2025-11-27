#include "transform.h"
#include "image.h"

#include <iostream>

Transform::Transform()
{
	transforms_.insert({TransformName::t11, Transform_X(/*TransformName::t11,*/ 3)});
	transforms_.insert({TransformName::f11, Transform_X(/*TransformName::f11,*/ 3)});
}

void Transform::t11(Image& image)
{
	Transform::Transform_X& transform_t11 = transforms_.find(TransformName::t11)->second;

	switch(transform_t11.current_step_number_)
	{
		case 0:
			transform_t11.transform_steps_[0].rotate(image, 90.0, 3000);
			transform_t11.transform_steps_[1].set_position(image, 200, -20, 3000);
			if(transform_t11.transform_steps_[0].transform_step_finished_ && transform_t11.transform_steps_[1].transform_step_finished_)
			{
				transform_t11.current_step_number_ = 1;
			}
			break;

		case 1:
			transform_t11.transform_steps_[2].night_filter(image, 2000);
			if(transform_t11.transform_steps_[2].transform_step_finished_)
			{
				transform_t11.current_step_number_ = 2;
				transform_t11.transform_finished_ = true;
			}
			break;

		default:
			break;
	}
}

void Transform::f11(Image& image)
{
	Transform::Transform_X& transform_f11 = transforms_.find(TransformName::f11)->second;

	switch(transform_f11.current_step_number_)
	{
		case 0:
			transform_f11.transform_steps_[0].zoom(image, 0.5f, 3000);
			transform_f11.transform_steps_[1].set_position(image, -50, 20, 3000);
			if(transform_f11.transform_steps_[0].transform_step_finished_ && transform_f11.transform_steps_[1].transform_step_finished_)
			{
				transform_f11.current_step_number_ = 1;
			}
			break;

		case 1:
			transform_f11.transform_steps_[2].afternoon_filter(image, 2000);
			if(transform_f11.transform_steps_[2].transform_step_finished_)
			{
				transform_f11.current_step_number_ = 2;
				transform_f11.transform_finished_ = true;
			}
			break;

		default:
			break;
	}
}

void Transform::show_transform(TransformName transform_name, Image& image)
{
	switch(transform_name)
	{
		case TransformName::t11:
			t11(image);
			break;

		case TransformName::f11:
			f11(image);
			break;

		default:
			break;
	}
}