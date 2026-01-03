#include "transform.h"
#include "image.h"

#include <iostream>

//TODO : "6" obligatoire ??

//Ce constructeur permet à Character::Editableproperties de disposer d'un constructeur par défaut
Transform::Transform()
	: is_character_visible_(false), transform_name_(""), /*previous_transform_name_(transform_name_),*/ transform_({"", TransformAllSteps(0)})
{}

Transform::Transform(const std::string transform_name)
	: is_character_visible_(false), transform_name_(transform_name), /*previous_transform_name_(transform_name_),*/ transform_({transform_name, TransformAllSteps(6)})
{}

std::string Transform::transform_to_focus()
{
	if(transform_name_.length() == 3 && transform_name_[0] == 't' && isdigit(transform_name_[1]) && isdigit(transform_name_[2]))
	{
		return std::string("f") + transform_name_[1] + transform_name_[2];
	}
	else
	{
		//it was already a focus transform or it is a non defaut DDLC transform
		return transform_name_;
	}
}

std::string Transform::transform_to_unfocus()
{
	if(transform_name_.length() == 3 && transform_name_[0] == 'f' && isdigit(transform_name_[1]) && isdigit(transform_name_[2]))
	{
		return std::string("t") + transform_name_[1] + transform_name_[2];
	}
	else
	{
		//it was already a non focus transform or it is a non defaut DDLC transform
		return transform_name_;
	}
}

void Transform::create_transform(const std::string transform_name, bool is_visible)
{
	transform_name_ = transform_name;
	is_character_visible_ = is_visible; //to choose between on show and on replace
	transform_ = {transform_name, TransformAllSteps(6)};
}

void Transform::show_transform(const std::string transform_name, Image& image)
{
	if(transform_name == "t11")
	{
		t11(image);
	}
	else if(transform_name == "t21")
	{
		t21(image);
	}
	else if(transform_name == "t22")
	{
		t22(image);
	}
	else if(transform_name == "t31")
	{
		t31(image);
	}
	else if(transform_name == "t32")
	{
		t32(image);
	}
	else if(transform_name == "t33")
	{
		t33(image);
	}
	else if(transform_name == "t41")
	{
		t41(image);
	}
	else if(transform_name == "t42")
	{
		t42(image);
	}
	else if(transform_name == "t43")
	{
		t43(image);
	}
	else if(transform_name == "t44")
	{
		t44(image);
	}
	else if(transform_name == "t51")
	{
		t51(image);
	}
	else if(transform_name == "t52")
	{
		t52(image);
	}
	else if(transform_name == "t53")
	{
		t53(image);
	}
	else if(transform_name == "t54")
	{
		t54(image);
	}
	else if(transform_name == "t55")
	{
		t55(image);
	}
	else if(transform_name == "f11")
	{
		f11(image);
	}
	else if(transform_name == "f21")
	{
		f21(image);
	}
	else if(transform_name == "f22")
	{
		f22(image);
	}
	else if(transform_name == "f31")
	{
		f31(image);
	}
	else if(transform_name == "f32")
	{
		f32(image);
	}
	else if(transform_name == "f33")
	{
		f33(image);
	}
	else if(transform_name == "f41")
	{
		f41(image);
	}
	else if(transform_name == "f42")
	{
		f42(image);
	}
	else if(transform_name == "f43")
	{
		f43(image);
	}
	else if(transform_name == "f44")
	{
		f44(image);
	}
	else if(transform_name == "f51")
	{
		f51(image);
	}
	else if(transform_name == "f52")
	{
		f52(image);
	}
	else if(transform_name == "f53")
	{
		f53(image);
	}
	else if(transform_name == "f54")
	{
		f54(image);
	}
	else if(transform_name == "f55")
	{
		f55(image);
	}
	else if(transform_name == "test")
	{
		test(image);
	}
	else if(transform_name == "hide")
	{
		hide(image);
	}
	else if(transform_name.empty() || transform_name == "none")
	{
		//do nothing!
	}
	else
	{
		std::cerr << "TRANSFORMATION " << transform_name << " NOT FOUND...\n";
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