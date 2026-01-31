#include "transform.h"
#include "image.h"

#include <iostream>

namespace
{

const int XPOS_11 = 640;

const int XPOS_21 = 400;
const int XPOS_22 = 880;

const int XPOS_31 = 240;
const int XPOS_32 = 640;
const int XPOS_33 = 1040;

const int XPOS_41 = 200;
const int XPOS_42 = 493;
const int XPOS_43 = 786;
const int XPOS_44 = 1080;

const int XPOS_51 = 140;
const int XPOS_52 = 390;
const int XPOS_53 = 640;
const int XPOS_54 = 890;
const int XPOS_55 = 1140;

}

//Ce constructeur permet à Character::Editableproperties de disposer d'un constructeur par défaut
Transform::Transform()
	: Transform("none", 0)
{}

Transform::Transform(std::string transform_name, int number_of_transform_steps)
	: is_character_visible_(false), transform_name_(transform_name), /*previous_transform_name_(transform_name_),*/ transform_({transform_name, TransformAllSteps(number_of_transform_steps)})
{}

void Transform::fill_all_transforms()
{
	if(all_transforms_.size() > 0) //TODO : l'idéal serait de rendre all_transforms_ static
	{
		return;
	}

	all_transforms_.insert({"t11", [this](Image& image){ t11(image); }});
	all_transforms_.insert({"t21", [this](Image& image){ t21(image); }});
	all_transforms_.insert({"t22", [this](Image& image){ t22(image); }});
	all_transforms_.insert({"t31", [this](Image& image){ t31(image); }});
	all_transforms_.insert({"t32", [this](Image& image){ t32(image); }});
	all_transforms_.insert({"t33", [this](Image& image){ t33(image); }});
	all_transforms_.insert({"t41", [this](Image& image){ t41(image); }});
	all_transforms_.insert({"t42", [this](Image& image){ t42(image); }});
	all_transforms_.insert({"t43", [this](Image& image){ t43(image); }});
	all_transforms_.insert({"t44", [this](Image& image){ t44(image); }});
	all_transforms_.insert({"t51", [this](Image& image){ t51(image); }});
	all_transforms_.insert({"t52", [this](Image& image){ t52(image); }});
	all_transforms_.insert({"t53", [this](Image& image){ t53(image); }});
	all_transforms_.insert({"t54", [this](Image& image){ t54(image); }});
	all_transforms_.insert({"t55", [this](Image& image){ t55(image); }});

	all_transforms_.insert({"f11", [this](Image& image){ f11(image); }});
	all_transforms_.insert({"f21", [this](Image& image){ f21(image); }});
	all_transforms_.insert({"f22", [this](Image& image){ f22(image); }});
	all_transforms_.insert({"f31", [this](Image& image){ f31(image); }});
	all_transforms_.insert({"f32", [this](Image& image){ f32(image); }});
	all_transforms_.insert({"f33", [this](Image& image){ f33(image); }});
	all_transforms_.insert({"f41", [this](Image& image){ f41(image); }});
	all_transforms_.insert({"f42", [this](Image& image){ f42(image); }});
	all_transforms_.insert({"f43", [this](Image& image){ f43(image); }});
	all_transforms_.insert({"f44", [this](Image& image){ f44(image); }});
	all_transforms_.insert({"f51", [this](Image& image){ f51(image); }});
	all_transforms_.insert({"f52", [this](Image& image){ f52(image); }});
	all_transforms_.insert({"f53", [this](Image& image){ f53(image); }});
	all_transforms_.insert({"f54", [this](Image& image){ f54(image); }});
	all_transforms_.insert({"f55", [this](Image& image){ f55(image); }});

	all_transforms_.insert({"h11", [this](Image& image){ h11(image); }});
	all_transforms_.insert({"h21", [this](Image& image){ h21(image); }});
	all_transforms_.insert({"h22", [this](Image& image){ h22(image); }});
	all_transforms_.insert({"h31", [this](Image& image){ h31(image); }});
	all_transforms_.insert({"h32", [this](Image& image){ h32(image); }});
	all_transforms_.insert({"h33", [this](Image& image){ h33(image); }});
	all_transforms_.insert({"h41", [this](Image& image){ h41(image); }});
	all_transforms_.insert({"h42", [this](Image& image){ h42(image); }});
	all_transforms_.insert({"h43", [this](Image& image){ h43(image); }});
	all_transforms_.insert({"h44", [this](Image& image){ h44(image); }});
	all_transforms_.insert({"h51", [this](Image& image){ h51(image); }});
	all_transforms_.insert({"h52", [this](Image& image){ h52(image); }});
	all_transforms_.insert({"h53", [this](Image& image){ h53(image); }});
	all_transforms_.insert({"h54", [this](Image& image){ h54(image); }});
	all_transforms_.insert({"h55", [this](Image& image){ h55(image); }});

	all_transforms_.insert({"hf11", [this](Image& image){ hf11(image); }});
	all_transforms_.insert({"hf21", [this](Image& image){ hf21(image); }});
	all_transforms_.insert({"hf22", [this](Image& image){ hf22(image); }});
	all_transforms_.insert({"hf31", [this](Image& image){ hf31(image); }});
	all_transforms_.insert({"hf32", [this](Image& image){ hf32(image); }});
	all_transforms_.insert({"hf33", [this](Image& image){ hf33(image); }});
	all_transforms_.insert({"hf41", [this](Image& image){ hf41(image); }});
	all_transforms_.insert({"hf42", [this](Image& image){ hf42(image); }});
	all_transforms_.insert({"hf43", [this](Image& image){ hf43(image); }});
	all_transforms_.insert({"hf44", [this](Image& image){ hf44(image); }});
	all_transforms_.insert({"hf51", [this](Image& image){ hf51(image); }});
	all_transforms_.insert({"hf52", [this](Image& image){ hf52(image); }});
	all_transforms_.insert({"hf53", [this](Image& image){ hf53(image); }});
	all_transforms_.insert({"hf54", [this](Image& image){ hf54(image); }});
	all_transforms_.insert({"hf55", [this](Image& image){ hf55(image); }});

	all_transforms_.insert({"s11", [this](Image& image){ s11(image); }});
	all_transforms_.insert({"s21", [this](Image& image){ s21(image); }});
	all_transforms_.insert({"s22", [this](Image& image){ s22(image); }});
	all_transforms_.insert({"s31", [this](Image& image){ s31(image); }});
	all_transforms_.insert({"s32", [this](Image& image){ s32(image); }});
	all_transforms_.insert({"s33", [this](Image& image){ s33(image); }});
	all_transforms_.insert({"s41", [this](Image& image){ s41(image); }});
	all_transforms_.insert({"s42", [this](Image& image){ s42(image); }});
	all_transforms_.insert({"s43", [this](Image& image){ s43(image); }});
	all_transforms_.insert({"s44", [this](Image& image){ s44(image); }});
	all_transforms_.insert({"s51", [this](Image& image){ s51(image); }});
	all_transforms_.insert({"s52", [this](Image& image){ s52(image); }});
	all_transforms_.insert({"s53", [this](Image& image){ s53(image); }});
	all_transforms_.insert({"s54", [this](Image& image){ s54(image); }});
	all_transforms_.insert({"s55", [this](Image& image){ s55(image); }});

	all_transforms_.insert({"d11", [this](Image& image){ d11(image); }});
	all_transforms_.insert({"d21", [this](Image& image){ d21(image); }});
	all_transforms_.insert({"d22", [this](Image& image){ d22(image); }});
	all_transforms_.insert({"d31", [this](Image& image){ d31(image); }});
	all_transforms_.insert({"d32", [this](Image& image){ d32(image); }});
	all_transforms_.insert({"d33", [this](Image& image){ d33(image); }});
	all_transforms_.insert({"d41", [this](Image& image){ d41(image); }});
	all_transforms_.insert({"d42", [this](Image& image){ d42(image); }});
	all_transforms_.insert({"d43", [this](Image& image){ d43(image); }});
	all_transforms_.insert({"d44", [this](Image& image){ d44(image); }});
	all_transforms_.insert({"d51", [this](Image& image){ d51(image); }});
	all_transforms_.insert({"d52", [this](Image& image){ d52(image); }});
	all_transforms_.insert({"d53", [this](Image& image){ d53(image); }});
	all_transforms_.insert({"d54", [this](Image& image){ d54(image); }});
	all_transforms_.insert({"d55", [this](Image& image){ d55(image); }});

	all_transforms_.insert({"hide", [this](Image& image){ hide(image); }});
	all_transforms_.insert({"lhide", [this](Image& image){ lhide(image); }});
	all_transforms_.insert({"rhide", [this](Image& image){ rhide(image); }});

	all_transforms_.insert({"test", [this](Image& image){ test(image); }});

	all_transforms_.insert({"none", nullptr}); //do nothing!
}

//TODO : ne fonctionne pas pour hop etc.
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

//TODO : ne fonctionne pas pour hop etc.
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

//TODO : "6" obligatoire ??
void Transform::create_transform(std::string transform_name, bool is_visible)
{
	transform_name_ = transform_name;
	is_character_visible_ = is_visible; //to choose between on show and on replace
	transform_ = {transform_name, TransformAllSteps(6)};
	fill_all_transforms();
}

void Transform::show_transform(std::string transform_name, Image& image)
{
	if(all_transforms_.count(transform_name))
	{
		all_transforms_[transform_name](image);
	}
	else
	{
		std::cerr << "TRANSFORMATION " << transform_name << " NOT FOUND...\n";
	}
}

void Transform::tcommon(int xpos, Image& image) 
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
					transform_t.transform_steps_[5].show(image, Duration(250, Duration::Kind::LINEAR)); 
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
	tcommon(XPOS_11, image);
}

void Transform::t21(Image& image)
{
	tcommon(XPOS_21, image);
}

void Transform::t22(Image& image)
{
	tcommon(XPOS_22, image);
}

void Transform::t31(Image& image)
{
	tcommon(XPOS_31, image);
}

void Transform::t32(Image& image)
{
	tcommon(XPOS_32, image);
}

void Transform::t33(Image& image)
{
	tcommon(XPOS_33, image);
}

void Transform::t41(Image& image)
{
	tcommon(XPOS_41, image);
}

void Transform::t42(Image& image)
{
	tcommon(XPOS_42, image);
}

void Transform::t43(Image& image)
{
	tcommon(XPOS_43, image);
}

void Transform::t44(Image& image)
{
	tcommon(XPOS_44, image);
}

void Transform::t51(Image& image)
{
	tcommon(XPOS_51, image);
}

void Transform::t52(Image& image)
{
	tcommon(XPOS_52, image);
}

void Transform::t53(Image& image)
{
	tcommon(XPOS_53, image);
}

void Transform::t54(Image& image)
{
	tcommon(XPOS_54, image);
}

void Transform::t55(Image& image)
{
	tcommon(XPOS_55, image);
}

void Transform::focus_common(int xpos, Image& image) 
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
					transform_f.transform_steps_[5].show(image, Duration(250, Duration::Kind::LINEAR));
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
	focus_common(XPOS_11, image);
}

void Transform::f21(Image& image)
{
	focus_common(XPOS_21, image);
}

void Transform::f22(Image& image)
{
	focus_common(XPOS_22, image);
}

void Transform::f31(Image& image)
{
	focus_common(XPOS_31, image);
}

void Transform::f32(Image& image)
{
	focus_common(XPOS_32, image);
}

void Transform::f33(Image& image)
{
	focus_common(XPOS_33, image);
}

void Transform::f41(Image& image)
{
	focus_common(XPOS_41, image);
}

void Transform::f42(Image& image)
{
	focus_common(XPOS_42, image);
}

void Transform::f43(Image& image)
{
	focus_common(XPOS_43, image);
}

void Transform::f44(Image& image)
{
	focus_common(XPOS_44, image);
}

void Transform::f51(Image& image)
{
	focus_common(XPOS_51, image);
}

void Transform::f52(Image& image)
{
	focus_common(XPOS_52, image);
}

void Transform::f53(Image& image)
{
	focus_common(XPOS_53, image);
}

void Transform::f54(Image& image)
{
	focus_common(XPOS_54, image);
}

void Transform::f55(Image& image)
{
	focus_common(XPOS_55, image);
}

void Transform::hop(int xpos, Image& image)
{
	Transform::TransformAllSteps& transform_h = transform_.second;

	//TODO : mettre ce if pour chaque transform ??
	if(!transform_h.transform_finished_)
	{
		switch(transform_h.current_step_number_)
		{
			case 0:
				//TODO : le bug disparaît quand la ligne du zoom est écrite en première, peu importe l'indice utilisé
				//=> n'est pas un "bug" mais le comportement normal. Il est logique que si j'applique d'abord set_center et ensuite un zoom, l'image ne sera plus centré mais l'ordre inverse, l'image sera centrée (cas time > 0)
				//=> Si je veux corriger ce problème, il faudrait que toutes les transfos avec time == 0 soit appliquée en même temps et non pas ligne par ligne
				
				transform_h.transform_steps_[0].reset(image);
				transform_h.transform_steps_[1].zoom(image, 0.8f);
				transform_h.transform_steps_[2].set_position_xcenter(image, xpos);
				transform_h.transform_steps_[3].set_position_yoffset(image, -26);
				
				if(transform_h.transform_steps_[0].transform_step_finished_
				&& transform_h.transform_steps_[1].transform_step_finished_
				&& transform_h.transform_steps_[2].transform_step_finished_
				&& transform_h.transform_steps_[3].transform_step_finished_)
				{
					transform_h.current_step_number_ += 1;
				}
				break;

			case 1:
				transform_h.transform_steps_[4].set_position_yoffset(image, -20, Duration(100, Duration::Kind::EASEIN));
				if(transform_h.transform_steps_[4].transform_step_finished_)
				{
					transform_h.current_step_number_ += 1;
				}
				break;

			case 2:
				transform_h.transform_steps_[5].set_position_yoffset(image, +20, Duration(100, Duration::Kind::EASEOUT));
				if(transform_h.transform_steps_[5].transform_step_finished_)
				{
					transform_h.current_step_number_ += 1;
					transform_h.transform_finished_ = true;
				}
				break;

			default:
				break;
		}
	}
}

void Transform::h11(Image& image)
{
	hop(XPOS_11, image);
}

void Transform::h21(Image& image)
{
	hop(XPOS_21, image);
}

void Transform::h22(Image& image)
{
	hop(XPOS_22, image);
}

void Transform::h31(Image& image)
{
	hop(XPOS_31, image);
}

void Transform::h32(Image& image)
{
	hop(XPOS_32, image);
}

void Transform::h33(Image& image)
{
	hop(XPOS_33, image);
}

void Transform::h41(Image& image)
{
	hop(XPOS_41, image);
}

void Transform::h42(Image& image)
{
	hop(XPOS_42, image);
}

void Transform::h43(Image& image)
{
	hop(XPOS_43, image);
}

void Transform::h44(Image& image)
{
	hop(XPOS_44, image);
}

void Transform::h51(Image& image)
{
	hop(XPOS_51, image);
}

void Transform::h52(Image& image)
{
	hop(XPOS_52, image);
}

void Transform::h53(Image& image)
{
	hop(XPOS_53, image);
}

void Transform::h54(Image& image)
{
	hop(XPOS_54, image);
}

void Transform::h55(Image& image)
{
	hop(XPOS_55, image);
}

void Transform::hop_focus(int xpos, Image& image)
{
	Transform::TransformAllSteps& transform_hf = transform_.second;

	//TODO : mettre ce if pour chaque transform ??
	if(!transform_hf.transform_finished_)
	{
		switch(transform_hf.current_step_number_)
		{
			case 0:
				//TODO : le bug disparaît quand la ligne du zoom est écrite en première, peu importe l'indice utilisé
				//=> n'est pas un "bug" mais le comportement normal. Il est logique que si j'applique d'abord set_center et ensuite un zoom, l'image ne sera plus centré mais l'ordre inverse, l'image sera centrée (cas time > 0)
				//=> Si je veux corriger ce problème, il faudrait que toutes les transfos avec time == 0 soit appliquée en même temps et non pas ligne par ligne

				transform_hf.transform_steps_[0].reset(image);
				transform_hf.transform_steps_[1].zoom(image, 0.84f);
				transform_hf.transform_steps_[2].set_position_xcenter(image, xpos);
				transform_hf.transform_steps_[3].set_position_yoffset(image, -65);

				if(transform_hf.transform_steps_[0].transform_step_finished_
				&& transform_hf.transform_steps_[1].transform_step_finished_
				&& transform_hf.transform_steps_[2].transform_step_finished_
				&& transform_hf.transform_steps_[3].transform_step_finished_)
				{
					transform_hf.current_step_number_ += 1;
				}
				break;

			case 1:
				transform_hf.transform_steps_[4].set_position_yoffset(image, -21, Duration(100, Duration::Kind::EASEIN));
				if(transform_hf.transform_steps_[4].transform_step_finished_)
				{
					transform_hf.current_step_number_ += 1;
				}
				break;

			case 2:
				transform_hf.transform_steps_[5].set_position_yoffset(image, +21, Duration(100, Duration::Kind::EASEOUT));
				if(transform_hf.transform_steps_[5].transform_step_finished_)
				{
					transform_hf.current_step_number_ += 1;
					transform_hf.transform_finished_ = true;
				}
				break;

			default:
				break;
		}
	}
}

void Transform::hf11(Image& image)
{
	hop_focus(XPOS_11, image);
}

void Transform::hf21(Image& image)
{
	hop_focus(XPOS_21, image);
}

void Transform::hf22(Image& image)
{
	hop_focus(XPOS_22, image);
}

void Transform::hf31(Image& image)
{
	hop_focus(XPOS_31, image);
}

void Transform::hf32(Image& image)
{
	hop_focus(XPOS_32, image);
}

void Transform::hf33(Image& image)
{
	hop_focus(XPOS_33, image);
}

void Transform::hf41(Image& image)
{
	hop_focus(XPOS_41, image);
}

void Transform::hf42(Image& image)
{
	hop_focus(XPOS_42, image);
}

void Transform::hf43(Image& image)
{
	hop_focus(XPOS_43, image);
}

void Transform::hf44(Image& image)
{
	hop_focus(XPOS_44, image);
}

void Transform::hf51(Image& image)
{
	hop_focus(XPOS_51, image);
}

void Transform::hf52(Image& image)
{
	hop_focus(XPOS_52, image);
}

void Transform::hf53(Image& image)
{
	hop_focus(XPOS_53, image);
}

void Transform::hf54(Image& image)
{
	hop_focus(XPOS_54, image);
}

void Transform::hf55(Image& image)
{
	hop_focus(XPOS_55, image);
}

void Transform::sink(int xpos, Image& image)
{
	Transform::TransformAllSteps& transform_s = transform_.second;

	//TODO : mettre ce if pour chaque transform ??
	if(!transform_s.transform_finished_)
	{
		switch(transform_s.current_step_number_)
		{
			case 0:
				//TODO : le bug disparaît quand la ligne du zoom est écrite en première, peu importe l'indice utilisé
				//=> n'est pas un "bug" mais le comportement normal. Il est logique que si j'applique d'abord set_center et ensuite un zoom, l'image ne sera plus centré mais l'ordre inverse, l'image sera centrée (cas time > 0)
				//=> Si je veux corriger ce problème, il faudrait que toutes les transfos avec time == 0 soit appliquée en même temps et non pas ligne par ligne

				transform_s.transform_steps_[0].reset(image);
				transform_s.transform_steps_[1].zoom(image, 0.8f);
				transform_s.transform_steps_[2].set_position_xcenter(image, xpos);
				transform_s.transform_steps_[3].set_position_yoffset(image, -26);

				if(transform_s.transform_steps_[0].transform_step_finished_
				&& transform_s.transform_steps_[1].transform_step_finished_
				&& transform_s.transform_steps_[2].transform_step_finished_
				&& transform_s.transform_steps_[3].transform_step_finished_)
				{
					transform_s.current_step_number_ += 1;
				}
				break;

			case 1:
				transform_s.transform_steps_[4].set_position_yoffset(image, +20, Duration(500, Duration::Kind::EASEIN));
				if(transform_s.transform_steps_[4].transform_step_finished_)
				{
					transform_s.current_step_number_ += 1;
					transform_s.transform_finished_ = true;
				}
				break;

			default:
				break;
		}
	}
}

void Transform::s11(Image& image)
{
	sink(XPOS_11, image);
}

void Transform::s21(Image& image)
{
	sink(XPOS_21, image);
}

void Transform::s22(Image& image)
{
	sink(XPOS_22, image);
}

void Transform::s31(Image& image)
{
	sink(XPOS_31, image);
}

void Transform::s32(Image& image)
{
	sink(XPOS_32, image);
}

void Transform::s33(Image& image)
{
	sink(XPOS_33, image);
}

void Transform::s41(Image& image)
{
	sink(XPOS_41, image);
}

void Transform::s42(Image& image)
{
	sink(XPOS_42, image);
}

void Transform::s43(Image& image)
{
	sink(XPOS_43, image);
}

void Transform::s44(Image& image)
{
	sink(XPOS_44, image);
}

void Transform::s51(Image& image)
{
	sink(XPOS_51, image);
}

void Transform::s52(Image& image)
{
	sink(XPOS_52, image);
}

void Transform::s53(Image& image)
{
	sink(XPOS_53, image);
}

void Transform::s54(Image& image)
{
	sink(XPOS_54, image);
}

void Transform::s55(Image& image)
{
	sink(XPOS_55, image);
}

void Transform::dip(int xpos, Image& image)
{
	Transform::TransformAllSteps& transform_d = transform_.second;

	//TODO : mettre ce if pour chaque transform ??
	if(!transform_d.transform_finished_)
	{
		switch(transform_d.current_step_number_)
		{
			case 0:
				//TODO : le bug disparaît quand la ligne du zoom est écrite en première, peu importe l'indice utilisé
				//=> n'est pas un "bug" mais le comportement normal. Il est logique que si j'applique d'abord set_center et ensuite un zoom, l'image ne sera plus centré mais l'ordre inverse, l'image sera centrée (cas time > 0)
				//=> Si je veux corriger ce problème, il faudrait que toutes les transfos avec time == 0 soit appliquée en même temps et non pas ligne par ligne

				transform_d.transform_steps_[0].reset(image);
				transform_d.transform_steps_[1].zoom(image, 0.8f);
				transform_d.transform_steps_[2].set_position_xcenter(image, xpos);
				transform_d.transform_steps_[3].set_position_yoffset(image, -26);

				if(transform_d.transform_steps_[0].transform_step_finished_
				&& transform_d.transform_steps_[1].transform_step_finished_
				&& transform_d.transform_steps_[2].transform_step_finished_
				&& transform_d.transform_steps_[3].transform_step_finished_)
				{
					transform_d.current_step_number_ += 1;
				}
				break;

			case 1:
				transform_d.transform_steps_[4].set_position_yoffset(image, +25, Duration(250, Duration::Kind::EASEIN));
				if(transform_d.transform_steps_[4].transform_step_finished_)
				{
					transform_d.current_step_number_ += 1;
				}
				break;

			case 2:
				transform_d.transform_steps_[5].set_position_yoffset(image, -25, Duration(250, Duration::Kind::EASEOUT));
				if(transform_d.transform_steps_[5].transform_step_finished_)
				{
					transform_d.current_step_number_ += 1;
					transform_d.transform_finished_ = true;
				}
				break;

			default:
				break;
		}
	}
}

void Transform::d11(Image& image)
{
	dip(XPOS_11, image);
}

void Transform::d21(Image& image)
{
	dip(XPOS_21, image);
}

void Transform::d22(Image& image)
{
	dip(XPOS_22, image);
}

void Transform::d31(Image& image)
{
	dip(XPOS_31, image);
}

void Transform::d32(Image& image)
{
	dip(XPOS_32, image);
}

void Transform::d33(Image& image)
{
	dip(XPOS_33, image);
}

void Transform::d41(Image& image)
{
	dip(XPOS_41, image);
}

void Transform::d42(Image& image)
{
	dip(XPOS_42, image);
}

void Transform::d43(Image& image)
{
	dip(XPOS_43, image);
}

void Transform::d44(Image& image)
{
	dip(XPOS_44, image);
}

void Transform::d51(Image& image)
{
	dip(XPOS_51, image);
}

void Transform::d52(Image& image)
{
	dip(XPOS_52, image);
}

void Transform::d53(Image& image)
{
	dip(XPOS_53, image);
}

void Transform::d54(Image& image)
{
	dip(XPOS_54, image);
}

void Transform::d55(Image& image)
{
	dip(XPOS_55, image);
}

void Transform::lhide(Image& image)
{
	Transform::TransformAllSteps& transform_l = transform_.second;

	//TODO : mettre ce if pour chaque transform ??
	if(!transform_l.transform_finished_)
	{
		switch(transform_l.current_step_number_)
		{
			case 0:
				//TODO : le bug disparaît quand la ligne du zoom est écrite en première, peu importe l'indice utilisé
				//=> n'est pas un "bug" mais le comportement normal. Il est logique que si j'applique d'abord set_center et ensuite un zoom, l'image ne sera plus centré mais l'ordre inverse, l'image sera centrée (cas time > 0)
				//=> Si je veux corriger ce problème, il faudrait que toutes les transfos avec time == 0 soit appliquée en même temps et non pas ligne par ligne

				transform_l.transform_steps_[0].set_position_xcenter(image, -300, Duration(250, Duration::Kind::EASEOUT));
				//image.flip_horizontally(); //TODO : inverser le sprite quand il va à gauche

				if(transform_l.transform_steps_[0].transform_step_finished_)
				{
					transform_l.current_step_number_ += 1;
					transform_l.transform_finished_ = true;
				}
				break;

			default:
				break;
		}
	}
}

void Transform::rhide(Image& image)
{
	Transform::TransformAllSteps& transform_r = transform_.second;

	//TODO : mettre ce if pour chaque transform ??
	if(!transform_r.transform_finished_)
	{
		switch(transform_r.current_step_number_)
		{
			case 0:
				//TODO : le bug disparaît quand la ligne du zoom est écrite en première, peu importe l'indice utilisé
				//=> n'est pas un "bug" mais le comportement normal. Il est logique que si j'applique d'abord set_center et ensuite un zoom, l'image ne sera plus centré mais l'ordre inverse, l'image sera centrée (cas time > 0)
				//=> Si je veux corriger ce problème, il faudrait que toutes les transfos avec time == 0 soit appliquée en même temps et non pas ligne par ligne

				transform_r.transform_steps_[0].set_position_xcenter(image, +2000, Duration(250, Duration::Kind::EASEOUT));
				//image.flip_horizontally(); //TODO : inverser le sprite quand il va à droite

				if(transform_r.transform_steps_[0].transform_step_finished_)
				{
					transform_r.current_step_number_ += 1;
					transform_r.transform_finished_ = true;
				}
				break;

			default:
				break;
		}
	}
}

void Transform::test(Image& image) 
{
	Transform::TransformAllSteps& transform_t = transform_.second;
	switch(transform_t.current_step_number_)
	{
		case 0:
			//transform_t.transform_steps_[0].zoom(image, 0.5f, 1000);
			//transform_t.transform_steps_[1].set_center(image, 3000);
			transform_t.transform_steps_[0].rotate(image, 360, Duration(3000, Duration::Kind::EASE));
			//transform_t.transform_steps_[3].set_alpha(image, 100, 3000);
			if(transform_t.transform_steps_[0].transform_step_finished_
			//&& transform_t.transform_steps_[1].transform_step_finished_
			/*&& transform_t.transform_steps_[2].transform_step_finished_
			&& transform_t.transform_steps_[3].transform_step_finished_*/)
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
			transform_t.transform_steps_[0].hide(image, Duration(250, Duration::Kind::LINEAR));
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