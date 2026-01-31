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
}

void Transform::show_transform(std::string transform_name, Image& image)
{
	//TODO : utiliser une std::unordered_map(std::string, std::function) ??

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
	else if(transform_name == "h11")
	{
		h11(image);
	}
	else if(transform_name == "h21")
	{
		h21(image);
	}
	else if(transform_name == "h22")
	{
		h22(image);
	}
	else if(transform_name == "h31")
	{
		h31(image);
	}
	else if(transform_name == "h32")
	{
		h32(image);
	}
	else if(transform_name == "h33")
	{
		h33(image);
	}
	else if(transform_name == "h41")
	{
		h41(image);
	}
	else if(transform_name == "h42")
	{
		h42(image);
	}
	else if(transform_name == "h43")
	{
		h43(image);
	}
	else if(transform_name == "h44")
	{
		h44(image);
	}
	else if(transform_name == "h51")
	{
		h51(image);
	}
	else if(transform_name == "h52")
	{
		h52(image);
	}
	else if(transform_name == "h53")
	{
		h53(image);
	}
	else if(transform_name == "h54")
	{
		h54(image);
	}
	else if(transform_name == "h55")
	{
		h55(image);
	}
	else if(transform_name == "hf11")
	{
		hf11(image);
	}
	else if(transform_name == "hf21")
	{
		hf21(image);
	}
	else if(transform_name == "hf22")
	{
		hf22(image);
	}
	else if(transform_name == "hf31")
	{
		hf31(image);
	}
	else if(transform_name == "hf32")
	{
		hf32(image);
	}
	else if(transform_name == "hf33")
	{
		hf33(image);
	}
	else if(transform_name == "hf41")
	{
		hf41(image);
	}
	else if(transform_name == "hf42")
	{
		hf42(image);
	}
	else if(transform_name == "hf43")
	{
		hf43(image);
	}
	else if(transform_name == "hf44")
	{
		hf44(image);
	}
	else if(transform_name == "hf51")
	{
		hf51(image);
	}
	else if(transform_name == "hf52")
	{
		hf52(image);
	}
	else if(transform_name == "hf53")
	{
		hf53(image);
	}
	else if(transform_name == "hf54")
	{
		hf54(image);
	}
	else if(transform_name == "hf55")
	{
		hf55(image);
	}
	else if(transform_name == "s11")
	{
		s11(image);
	}
	else if(transform_name == "s21")
	{
		s21(image);
	}
	else if(transform_name == "s22")
	{
		s22(image);
	}
	else if(transform_name == "s31")
	{
		s31(image);
	}
	else if(transform_name == "s32")
	{
		s32(image);
	}
	else if(transform_name == "s33")
	{
		s33(image);
	}
	else if(transform_name == "s41")
	{
		s41(image);
	}
	else if(transform_name == "s42")
	{
		s42(image);
	}
	else if(transform_name == "s43")
	{
		s43(image);
	}
	else if(transform_name == "s44")
	{
		s44(image);
	}
	else if(transform_name == "s51")
	{
		s51(image);
	}
	else if(transform_name == "s52")
	{
		s52(image);
	}
	else if(transform_name == "s53")
	{
		s53(image);
	}
	else if(transform_name == "s54")
	{
		s54(image);
	}
	else if(transform_name == "s55")
	{
		s55(image);
	}
	else if(transform_name == "d11")
	{
		d11(image);
	}
	else if(transform_name == "d21")
	{
		d21(image);
	}
	else if(transform_name == "d22")
	{
		d22(image);
	}
	else if(transform_name == "d31")
	{
		d31(image);
	}
	else if(transform_name == "d32")
	{
		d32(image);
	}
	else if(transform_name == "d33")
	{
		d33(image);
	}
	else if(transform_name == "d41")
	{
		d41(image);
	}
	else if(transform_name == "d42")
	{
		d42(image);
	}
	else if(transform_name == "d43")
	{
		d43(image);
	}
	else if(transform_name == "d44")
	{
		d44(image);
	}
	else if(transform_name == "d51")
	{
		d51(image);
	}
	else if(transform_name == "d52")
	{
		d52(image);
	}
	else if(transform_name == "d53")
	{
		d53(image);
	}
	else if(transform_name == "d54")
	{
		d54(image);
	}
	else if(transform_name == "d55")
	{
		d55(image);
	}
	else if(transform_name == "lhide")
	{
		lhide(image);
	}
	else if(transform_name == "rhide")
	{
		rhide(image);
	}
	else if(transform_name == "test")
	{
		test(image);
	}
	else if(transform_name == "hide")
	{
		hide(image);
	}
	else if(transform_name == "none")
	{
		//do nothing!
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