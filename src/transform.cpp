#include "transform.h"
#include "image.h"

#include <iostream>

Transform::Transform()
{
	transforms_.insert({TransformName::t11, Transform_X(/*TransformName::t11,*/ 5)});
	transforms_.insert({TransformName::f11, Transform_X(/*TransformName::f11,*/ 5)});
}

void Transform::t11(Image& image)
{
	Transform::Transform_X& transform_t11 = transforms_.find(TransformName::t11)->second;

	switch(transform_t11.current_step_number_)
	{
		case 0:
			transform_t11.transform_steps_[0].hide(image);
			transform_t11.transform_steps_[1].set_position_yoffset(image, -26);
			transform_t11.transform_steps_[2].zoom(image, 0.8f); 
			transform_t11.transform_steps_[3].set_position_xcenter(image, 640);
			if(transform_t11.transform_steps_[0].transform_step_finished_ 
			&& transform_t11.transform_steps_[1].transform_step_finished_ 
			&& transform_t11.transform_steps_[2].transform_step_finished_ 
			&& transform_t11.transform_steps_[3].transform_step_finished_)
			{
				transform_t11.current_step_number_ += 1;
			}
			break;

		case 1:
			transform_t11.transform_steps_[4].show(image, 250);
			if(transform_t11.transform_steps_[4].transform_step_finished_)
			{
				transform_t11.current_step_number_ += 1;
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
			transform_f11.transform_steps_[0].hide(image);
			transform_f11.transform_steps_[1].set_position_yoffset(image, -65); 
			transform_f11.transform_steps_[2].zoom(image, 0.84f); 
			transform_f11.transform_steps_[3].set_position_xcenter(image, 640);
			if(transform_f11.transform_steps_[0].transform_step_finished_ 
			&& transform_f11.transform_steps_[1].transform_step_finished_ 
			&& transform_f11.transform_steps_[2].transform_step_finished_ 
			&& transform_f11.transform_steps_[3].transform_step_finished_)
			{
				transform_f11.current_step_number_ += 1;
			}
			break;

		case 1:
			transform_f11.transform_steps_[4].show(image, 250);
			if(transform_f11.transform_steps_[4].transform_step_finished_)
			{
				transform_f11.current_step_number_ += 1;
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