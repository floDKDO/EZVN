#include "transform.h"
#include "image.h"

#include <iostream>

//TODO : "6" obligatoire ??

Transform::Transform(const TransformName transform_name)
	: is_character_visible_(false), transform_name_(transform_name), previous_transform_name_(transform_name_), transform_({transform_name, TransformAllSteps(6)})
{}

void Transform::create_transform(const TransformName transform_name, bool is_visible)
{
	transform_name_ = transform_name;
	is_character_visible_ = is_visible; //to choose between on show and on replace
	transform_ = {transform_name, TransformAllSteps(6)};
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

		case TransformName::t51:
			t51(image);
			break;

		case TransformName::t52:
			t52(image);
			break;

		case TransformName::t53:
			t53(image);
			break;

		case TransformName::t54:
			t54(image);
			break;

		case TransformName::t55:
			t55(image);
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

		case TransformName::f51:
			f51(image);
			break;

		case TransformName::f52:
			f52(image);
			break;

		case TransformName::f53:
			f53(image);
			break;

		case TransformName::f54:
			f54(image);
			break;

		case TransformName::f55:
			f55(image);
			break;

		case TransformName::test:
			test(image);
			break;

		case TransformName::hide:
			hide(image);
			break;

		case TransformName::none:
			break;

		default:
			std::cerr << "TRANSFORMATION NOT FOUND...\n";
			break;
	}
}

void Transform::tcommon(const int xpos, Image& image) 
{
	Transform::TransformAllSteps& transform_t = transform_.second;

	//TODO : mettre ce if pour chaque transform ??
	if(!transform_t.transform_finished_)
	{
		if(is_character_visible_) //<=> on replace
		{
			switch(transform_t.current_step_number_)
			{
				case 0:
					transform_t.transform_steps_[0].reset(image);
					transform_t.transform_steps_[1].set_alpha(image, 255);
					transform_t.transform_steps_[2].set_position_yoffset(image, -26);
					transform_t.transform_steps_[3].zoom(image, 0.8f);
					transform_t.transform_steps_[4].set_position_xcenter(image, xpos);
					if(transform_t.transform_steps_[0].transform_step_finished_
					&& transform_t.transform_steps_[1].transform_step_finished_
					&& transform_t.transform_steps_[2].transform_step_finished_
					&& transform_t.transform_steps_[3].transform_step_finished_
					&& transform_t.transform_steps_[4].transform_step_finished_)
					{
						transform_t.current_step_number_ += 1;
						transform_t.transform_finished_ = true;
					}
					break;

				default:
					break;
			}
		}
		else //<=> on show
		{
			switch(transform_t.current_step_number_)
			{
				case 0:
					transform_t.transform_steps_[0].reset(image);
					transform_t.transform_steps_[1].hide(image);
					transform_t.transform_steps_[2].set_position_yoffset(image, -26);
					transform_t.transform_steps_[3].zoom(image, 0.8f);
					transform_t.transform_steps_[4].set_position_xcenter(image, xpos);
					if(transform_t.transform_steps_[0].transform_step_finished_
					&& transform_t.transform_steps_[1].transform_step_finished_
					&& transform_t.transform_steps_[2].transform_step_finished_
					&& transform_t.transform_steps_[3].transform_step_finished_
					&& transform_t.transform_steps_[4].transform_step_finished_)
					{
						transform_t.current_step_number_ += 1;
					}
					break;

				case 1:
					transform_t.transform_steps_[5].show(image, 250);
					if(transform_t.transform_steps_[5].transform_step_finished_)
					{
						transform_t.current_step_number_ += 1;
						transform_t.transform_finished_ = true;
					}
					break;

				default:
					break;
			}
		}
	}
}

void Transform::t11(Image& image)
{
	tcommon(640, image);
}

void Transform::t21(Image& image)
{
	tcommon(400, image);
}

void Transform::t22(Image& image)
{
	tcommon(880, image);
}

void Transform::t31(Image& image)
{
	tcommon(240, image);
}

void Transform::t32(Image& image)
{
	tcommon(640, image);
}

void Transform::t33(Image& image)
{
	tcommon(1040, image);
}

void Transform::t41(Image& image)
{
	tcommon(200, image);
}

void Transform::t42(Image& image)
{
	tcommon(493, image);
}

void Transform::t43(Image& image)
{
	tcommon(786, image);
}

void Transform::t44(Image& image)
{
	tcommon(1080, image);
}

void Transform::t51(Image& image)
{
	tcommon(140, image);
}

void Transform::t52(Image& image)
{
	tcommon(390, image);
}

void Transform::t53(Image& image)
{
	tcommon(640, image);
}

void Transform::t54(Image& image)
{
	tcommon(890, image);
}

void Transform::t55(Image& image)
{
	tcommon(1140, image);
}

void Transform::focus_common(const int xpos, Image& image) 
{
	Transform::TransformAllSteps& transform_f = transform_.second;

	//TODO : mettre ce if pour chaque transform ??
	if(!transform_f.transform_finished_)
	{
		if(is_character_visible_) //<=> on replace
		{
			switch(transform_f.current_step_number_)
			{
				case 0:
					transform_f.transform_steps_[0].reset(image);
					transform_f.transform_steps_[1].set_alpha(image, 255);
					transform_f.transform_steps_[2].set_position_yoffset(image, -65);
					transform_f.transform_steps_[3].zoom(image, 0.84f);
					transform_f.transform_steps_[4].set_position_xcenter(image, xpos);
					if(transform_f.transform_steps_[0].transform_step_finished_
					&& transform_f.transform_steps_[1].transform_step_finished_
					&& transform_f.transform_steps_[2].transform_step_finished_
					&& transform_f.transform_steps_[3].transform_step_finished_
					&& transform_f.transform_steps_[4].transform_step_finished_)
					{
						transform_f.current_step_number_ += 1;
						transform_f.transform_finished_ = true;
					}
					break;

				default:
					break;
			}
		}
		else //<=> on show
		{
			switch(transform_f.current_step_number_)
			{
				case 0:
					transform_f.transform_steps_[0].reset(image);
					transform_f.transform_steps_[1].hide(image);
					transform_f.transform_steps_[2].set_position_yoffset(image, -65);
					transform_f.transform_steps_[3].zoom(image, 0.84f);
					transform_f.transform_steps_[4].set_position_xcenter(image, xpos);
					if(transform_f.transform_steps_[0].transform_step_finished_
					&& transform_f.transform_steps_[1].transform_step_finished_
					&& transform_f.transform_steps_[2].transform_step_finished_
					&& transform_f.transform_steps_[3].transform_step_finished_
					&& transform_f.transform_steps_[4].transform_step_finished_)
					{
						transform_f.current_step_number_ += 1;
					}
					break;

				case 1:
					transform_f.transform_steps_[5].show(image, 250);
					if(transform_f.transform_steps_[5].transform_step_finished_)
					{
						transform_f.current_step_number_ += 1;
						transform_f.transform_finished_ = true;
					}
					break;

				default:
					break;
			}
		}
	}
}

void Transform::f11(Image& image)
{
	focus_common(640, image);
}

void Transform::f21(Image& image)
{
	focus_common(400, image);
}

void Transform::f22(Image& image)
{
	focus_common(880, image);
}

void Transform::f31(Image& image)
{
	focus_common(240, image);
}

void Transform::f32(Image& image)
{
	focus_common(640, image);
}

void Transform::f33(Image& image)
{
	focus_common(1040, image);
}

void Transform::f41(Image& image)
{
	focus_common(200, image);
}

void Transform::f42(Image& image)
{
	focus_common(493, image);
}

void Transform::f43(Image& image)
{
	focus_common(786, image);
}

void Transform::f44(Image& image)
{
	focus_common(1080, image);
}

void Transform::f51(Image& image)
{
	focus_common(140, image);
}

void Transform::f52(Image& image)
{
	focus_common(390, image);
}

void Transform::f53(Image& image)
{
	focus_common(640, image);
}

void Transform::f54(Image& image)
{
	focus_common(890, image);
}

void Transform::f55(Image& image)
{
	focus_common(1140, image);
}

void Transform::test(Image& image) 
{
	Transform::TransformAllSteps& transform_t = transform_.second;
	switch(transform_t.current_step_number_)
	{
		case 0:
			//transform_t.transform_steps_[0].zoom(image, 0.5f, 3000);
			transform_t.transform_steps_[1].set_position_yoffset(image, 100, 3000);
			if(/*transform_t.transform_steps_[0].transform_step_finished_
			&&*/ transform_t.transform_steps_[1].transform_step_finished_)
			{
				transform_t.current_step_number_ += 1;
				transform_t.transform_finished_ = true;
			}
			break;

		default:
			break;
	}
}

void Transform::hide(Image& image) //TODO : paramètre optionnel time
{
	Transform::TransformAllSteps& transform_t = transform_.second;
	switch(transform_t.current_step_number_)
	{
		case 0:
			transform_t.transform_steps_[0].hide(image, 250);
			if(transform_t.transform_steps_[0].transform_step_finished_)
			{
				transform_t.current_step_number_ += 1;
				transform_t.transform_finished_ = true;
			}
			break;

		default:
			break;
	}
}