#include "transform.h"
#include "image.h"

#include <iostream>

//TODO : "5" obligatoire ??
Transform::Transform()
{
	transforms_.insert({TransformName::t11, TransformAllSteps(/*TransformName::t11,*/ 5)});
	transforms_.insert({TransformName::t21, TransformAllSteps(/*TransformName::t21,*/ 5)});
	transforms_.insert({TransformName::t22, TransformAllSteps(/*TransformName::t22,*/ 5)});
	transforms_.insert({TransformName::t31, TransformAllSteps(/*TransformName::t31,*/ 5)});
	transforms_.insert({TransformName::t32, TransformAllSteps(/*TransformName::t32,*/ 5)});
	transforms_.insert({TransformName::t33, TransformAllSteps(/*TransformName::t33,*/ 5)});
	transforms_.insert({TransformName::t41, TransformAllSteps(/*TransformName::t41,*/ 5)});
	transforms_.insert({TransformName::t42, TransformAllSteps(/*TransformName::t42,*/ 5)});
	transforms_.insert({TransformName::t43, TransformAllSteps(/*TransformName::t43,*/ 5)});
	transforms_.insert({TransformName::t44, TransformAllSteps(/*TransformName::t44,*/ 5)});

	transforms_.insert({TransformName::f11, TransformAllSteps(/*TransformName::f11,*/ 5)});
	transforms_.insert({TransformName::f21, TransformAllSteps(/*TransformName::f21,*/ 5)});
	transforms_.insert({TransformName::f22, TransformAllSteps(/*TransformName::f22,*/ 5)});
	transforms_.insert({TransformName::f31, TransformAllSteps(/*TransformName::f31,*/ 5)});
	transforms_.insert({TransformName::f32, TransformAllSteps(/*TransformName::f32,*/ 5)});
	transforms_.insert({TransformName::f33, TransformAllSteps(/*TransformName::f33,*/ 5)});
	transforms_.insert({TransformName::f41, TransformAllSteps(/*TransformName::f41,*/ 5)});
	transforms_.insert({TransformName::f42, TransformAllSteps(/*TransformName::f42,*/ 5)});
	transforms_.insert({TransformName::f43, TransformAllSteps(/*TransformName::f43,*/ 5)});
	transforms_.insert({TransformName::f44, TransformAllSteps(/*TransformName::f44,*/ 5)});
}

void Transform::tcommon(const int xpos, Image& image, Transform::TransformAllSteps& transform_t)
{
	switch(transform_t.current_step_number_)
	{
		case 0:
			transform_t.transform_steps_[0].hide(image);
			transform_t.transform_steps_[1].set_position_yoffset(image, -26);
			transform_t.transform_steps_[2].zoom(image, 0.8f);
			transform_t.transform_steps_[3].set_position_xcenter(image, xpos);
			if(transform_t.transform_steps_[0].transform_step_finished_
			&& transform_t.transform_steps_[1].transform_step_finished_
			&& transform_t.transform_steps_[2].transform_step_finished_
			&& transform_t.transform_steps_[3].transform_step_finished_)
			{
				transform_t.current_step_number_ += 1;
			}
			break;

		case 1:
			transform_t.transform_steps_[4].show(image, 250);
			if(transform_t.transform_steps_[4].transform_step_finished_)
			{
				transform_t.current_step_number_ += 1;
				transform_t.transform_finished_ = true;
			}
			break;

		default:
			break;
	}

}

void Transform::t11(Image& image)
{
	Transform::TransformAllSteps& transform_t11 = transforms_.find(TransformName::t11)->second;
	tcommon(640, image, transform_t11);
}

void Transform::t21(Image& image)
{
	Transform::TransformAllSteps& transform_t21 = transforms_.find(TransformName::t21)->second;
	tcommon(400, image, transform_t21);
}

void Transform::t22(Image& image)
{
	Transform::TransformAllSteps& transform_t22 = transforms_.find(TransformName::t22)->second;
	tcommon(880, image, transform_t22);
}

void Transform::t31(Image& image)
{
	Transform::TransformAllSteps& transform_t31 = transforms_.find(TransformName::t31)->second;
	tcommon(240, image, transform_t31);
}

void Transform::t32(Image& image)
{
	Transform::TransformAllSteps& transform_t32 = transforms_.find(TransformName::t32)->second;
	tcommon(640, image, transform_t32);
}

void Transform::t33(Image& image)
{
	Transform::TransformAllSteps& transform_t33 = transforms_.find(TransformName::t33)->second;
	tcommon(1040, image, transform_t33);
}

void Transform::t41(Image& image)
{
	Transform::TransformAllSteps& transform_t41 = transforms_.find(TransformName::t41)->second;
	tcommon(200, image, transform_t41);
}

void Transform::t42(Image& image)
{
	Transform::TransformAllSteps& transform_t42 = transforms_.find(TransformName::t42)->second;
	tcommon(493, image, transform_t42);
}

void Transform::t43(Image& image)
{
	Transform::TransformAllSteps& transform_t43 = transforms_.find(TransformName::t43)->second;
	tcommon(786, image, transform_t43);
}

void Transform::t44(Image& image)
{
	Transform::TransformAllSteps& transform_t44 = transforms_.find(TransformName::t44)->second;
	tcommon(1080, image, transform_t44);
}

void Transform::focus_common(const int xpos, Image& image, Transform::TransformAllSteps& transform_f)
{
	switch(transform_f.current_step_number_)
	{
		case 0:
			transform_f.transform_steps_[0].hide(image);
			transform_f.transform_steps_[1].set_position_yoffset(image, -65);
			transform_f.transform_steps_[2].zoom(image, 0.84f);
			transform_f.transform_steps_[3].set_position_xcenter(image, xpos);
			if(transform_f.transform_steps_[0].transform_step_finished_
			&& transform_f.transform_steps_[1].transform_step_finished_
			&& transform_f.transform_steps_[2].transform_step_finished_
			&& transform_f.transform_steps_[3].transform_step_finished_)
			{
				transform_f.current_step_number_ += 1;
			}
			break;

		case 1:
			transform_f.transform_steps_[4].show(image, 250);
			if(transform_f.transform_steps_[4].transform_step_finished_)
			{
				transform_f.current_step_number_ += 1;
				transform_f.transform_finished_ = true;
			}
			break;

		default:
			break;
	}
}

void Transform::f11(Image& image)
{
	Transform::TransformAllSteps& transform_f11 = transforms_.find(TransformName::f11)->second;
	focus_common(640, image, transform_f11);
}

void Transform::f21(Image& image)
{
	Transform::TransformAllSteps& transform_f21 = transforms_.find(TransformName::f21)->second;
	focus_common(400, image, transform_f21);
}

void Transform::f22(Image& image)
{
	Transform::TransformAllSteps& transform_f22 = transforms_.find(TransformName::f22)->second;
	focus_common(880, image, transform_f22);
}

void Transform::f31(Image& image)
{
	Transform::TransformAllSteps& transform_f31 = transforms_.find(TransformName::f31)->second;
	focus_common(240, image, transform_f31);
}

void Transform::f32(Image& image)
{
	Transform::TransformAllSteps& transform_f32 = transforms_.find(TransformName::f32)->second;
	focus_common(640, image, transform_f32);
}

void Transform::f33(Image& image)
{
	Transform::TransformAllSteps& transform_f33 = transforms_.find(TransformName::f33)->second;
	focus_common(1040, image, transform_f33);
}

void Transform::f41(Image& image)
{
	Transform::TransformAllSteps& transform_f41 = transforms_.find(TransformName::f41)->second;
	focus_common(200, image, transform_f41);
}

void Transform::f42(Image& image)
{
	Transform::TransformAllSteps& transform_f42 = transforms_.find(TransformName::f42)->second;
	focus_common(493, image, transform_f42);
}

void Transform::f43(Image& image)
{
	Transform::TransformAllSteps& transform_f43 = transforms_.find(TransformName::f43)->second;
	focus_common(786, image, transform_f43);
}

void Transform::f44(Image& image)
{
	Transform::TransformAllSteps& transform_f44 = transforms_.find(TransformName::f44)->second;
	focus_common(1080, image, transform_f44);
}

void Transform::show_transform(const TransformName transform_name, Image& image)
{
	switch(transform_name)
	{
		case TransformName::t11:
			t11(image);
			break;

		case TransformName::t21: 
			t21(image);
			break;

		case TransformName::t22: 
			t22(image);
			break;

		case TransformName::t31: 
			t31(image);
			break;

		case TransformName::t32: 
			t32(image);
			break;

		case TransformName::t33: 
			t33(image);
			break;

		case TransformName::t41: 
			t41(image);
			break;

		case TransformName::t42: 
			t42(image);
			break;

		case TransformName::t43:
			t43(image);
			break;

		case TransformName::t44:
			t44(image);
			break;

		case TransformName::f11:
			f11(image);
			break;

		case TransformName::f21: 
			f21(image);
			break;

		case TransformName::f22: 
			f22(image);
			break;

		case TransformName::f31: 
			f31(image);
			break;

		case TransformName::f32: 
			f32(image);
			break;

		case TransformName::f33: 
			f33(image);
			break;

		case TransformName::f41:
			f41(image);
			break;

		case TransformName::f42: 
			f42(image);
			break;

		case TransformName::f43:
			f43(image);
			break;

		case TransformName::f44: 
			f44(image);
			break;

		default:
			break;
	}
}