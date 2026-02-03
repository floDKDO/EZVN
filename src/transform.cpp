#include "transform.h"
#include "image.h"

#include <iostream>

//anonymous namespace
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
	: Transform("none", 0, false)
{}

Transform::Transform(std::string transform_name, int number_of_transform_steps, bool is_character_visible)
	: is_character_visible_(is_character_visible) /*to choose between on show and on replace*/, transform_name_(transform_name), current_line_(0), is_finished_(false)/*, transform_steps_(1) , previous_transform_name_(transform_name_),*/ 
	//transform_({transform_name, TransformAllSteps(number_of_transform_steps)}) //TODO : "number_of_transform_steps" obligatoire ??
{
	//fill_all_transforms();
}

void Transform::fill_all_transforms(/*Image& image*/)
{
	if(all_transforms_.size() > 0) //TODO : l'idéal serait de rendre all_transforms_ static
	{
		return;
	}

	std::cout << this << std::endl;

	all_build_methods_.insert({"t11", [this](Image& image){ t11(image); }});
	all_build_methods_.insert({"t21", [this](Image& image){ t21(image); }});
	all_build_methods_.insert({"t22", [this](Image& image){ t22(image); }});
	all_build_methods_.insert({"t31", [this](Image& image){ t31(image); }});
	all_build_methods_.insert({"t32", [this](Image& image){ t32(image); }});
	all_build_methods_.insert({"t33", [this](Image& image){ t33(image); }});
	all_build_methods_.insert({"t41", [this](Image& image){ t41(image); }});
	all_build_methods_.insert({"t42", [this](Image& image){ t42(image); }});
	all_build_methods_.insert({"t43", [this](Image& image){ t43(image); }});
	all_build_methods_.insert({"t44", [this](Image& image){ t44(image); }});
	all_build_methods_.insert({"t51", [this](Image& image){ t51(image); }});
	all_build_methods_.insert({"t52", [this](Image& image){ t52(image); }});
	all_build_methods_.insert({"t53", [this](Image& image){ t53(image); }});
	all_build_methods_.insert({"t54", [this](Image& image){ t54(image); }});
	all_build_methods_.insert({"t55", [this](Image& image){ t55(image); }});

	all_build_methods_.insert({"f11", [this](Image& image){ f11(image); }});
	all_build_methods_.insert({"f21", [this](Image& image){ f21(image); }});
	all_build_methods_.insert({"f22", [this](Image& image){ f22(image); }});
	all_build_methods_.insert({"f31", [this](Image& image){ f31(image); }});
	all_build_methods_.insert({"f32", [this](Image& image){ f32(image); }});
	all_build_methods_.insert({"f33", [this](Image& image){ f33(image); }});
	all_build_methods_.insert({"f41", [this](Image& image){ f41(image); }});
	all_build_methods_.insert({"f42", [this](Image& image){ f42(image); }});
	all_build_methods_.insert({"f43", [this](Image& image){ f43(image); }});
	all_build_methods_.insert({"f44", [this](Image& image){ f44(image); }});
	all_build_methods_.insert({"f51", [this](Image& image){ f51(image); }});
	all_build_methods_.insert({"f52", [this](Image& image){ f52(image); }});
	all_build_methods_.insert({"f53", [this](Image& image){ f53(image); }});
	all_build_methods_.insert({"f54", [this](Image& image){ f54(image); }});
	all_build_methods_.insert({"f55", [this](Image& image){ f55(image); }});

	all_build_methods_.insert({"h11", [this](Image& image){ h11(image); }});
	all_build_methods_.insert({"h21", [this](Image& image){ h21(image); }});
	all_build_methods_.insert({"h22", [this](Image& image){ h22(image); }});
	all_build_methods_.insert({"h31", [this](Image& image){ h31(image); }});
	all_build_methods_.insert({"h32", [this](Image& image){ h32(image); }});
	all_build_methods_.insert({"h33", [this](Image& image){ h33(image); }});
	all_build_methods_.insert({"h41", [this](Image& image){ h41(image); }});
	all_build_methods_.insert({"h42", [this](Image& image){ h42(image); }});
	all_build_methods_.insert({"h43", [this](Image& image){ h43(image); }});
	all_build_methods_.insert({"h44", [this](Image& image){ h44(image); }});
	all_build_methods_.insert({"h51", [this](Image& image){ h51(image); }});
	all_build_methods_.insert({"h52", [this](Image& image){ h52(image); }});
	all_build_methods_.insert({"h53", [this](Image& image){ h53(image); }});
	all_build_methods_.insert({"h54", [this](Image& image){ h54(image); }});
	all_build_methods_.insert({"h55", [this](Image& image){ h55(image); }});

	all_build_methods_.insert({"hf11", [this](Image& image){ hf11(image); }});
	all_build_methods_.insert({"hf21", [this](Image& image){ hf21(image); }});
	all_build_methods_.insert({"hf22", [this](Image& image){ hf22(image); }});
	all_build_methods_.insert({"hf31", [this](Image& image){ hf31(image); }});
	all_build_methods_.insert({"hf32", [this](Image& image){ hf32(image); }});
	all_build_methods_.insert({"hf33", [this](Image& image){ hf33(image); }});
	all_build_methods_.insert({"hf41", [this](Image& image){ hf41(image); }});
	all_build_methods_.insert({"hf42", [this](Image& image){ hf42(image); }});
	all_build_methods_.insert({"hf43", [this](Image& image){ hf43(image); }});
	all_build_methods_.insert({"hf44", [this](Image& image){ hf44(image); }});
	all_build_methods_.insert({"hf51", [this](Image& image){ hf51(image); }});
	all_build_methods_.insert({"hf52", [this](Image& image){ hf52(image); }});
	all_build_methods_.insert({"hf53", [this](Image& image){ hf53(image); }});
	all_build_methods_.insert({"hf54", [this](Image& image){ hf54(image); }});
	all_build_methods_.insert({"hf55", [this](Image& image){ hf55(image); }});

	all_build_methods_.insert({"s11", [this](Image& image){ s11(image); }});
	all_build_methods_.insert({"s21", [this](Image& image){ s21(image); }});
	all_build_methods_.insert({"s22", [this](Image& image){ s22(image); }});
	all_build_methods_.insert({"s31", [this](Image& image){ s31(image); }});
	all_build_methods_.insert({"s32", [this](Image& image){ s32(image); }});
	all_build_methods_.insert({"s33", [this](Image& image){ s33(image); }});
	all_build_methods_.insert({"s41", [this](Image& image){ s41(image); }});
	all_build_methods_.insert({"s42", [this](Image& image){ s42(image); }});
	all_build_methods_.insert({"s43", [this](Image& image){ s43(image); }});
	all_build_methods_.insert({"s44", [this](Image& image){ s44(image); }});
	all_build_methods_.insert({"s51", [this](Image& image){ s51(image); }});
	all_build_methods_.insert({"s52", [this](Image& image){ s52(image); }});
	all_build_methods_.insert({"s53", [this](Image& image){ s53(image); }});
	all_build_methods_.insert({"s54", [this](Image& image){ s54(image); }});
	all_build_methods_.insert({"s55", [this](Image& image){ s55(image); }});

	all_build_methods_.insert({"d11", [this](Image& image){ d11(image); }});
	all_build_methods_.insert({"d21", [this](Image& image){ d21(image); }});
	all_build_methods_.insert({"d22", [this](Image& image){ d22(image); }});
	all_build_methods_.insert({"d31", [this](Image& image){ d31(image); }});
	all_build_methods_.insert({"d32", [this](Image& image){ d32(image); }});
	all_build_methods_.insert({"d33", [this](Image& image){ d33(image); }});
	all_build_methods_.insert({"d41", [this](Image& image){ d41(image); }});
	all_build_methods_.insert({"d42", [this](Image& image){ d42(image); }});
	all_build_methods_.insert({"d43", [this](Image& image){ d43(image); }});
	all_build_methods_.insert({"d44", [this](Image& image){ d44(image); }});
	all_build_methods_.insert({"d51", [this](Image& image){ d51(image); }});
	all_build_methods_.insert({"d52", [this](Image& image){ d52(image); }});
	all_build_methods_.insert({"d53", [this](Image& image){ d53(image); }});
	all_build_methods_.insert({"d54", [this](Image& image){ d54(image); }});
	all_build_methods_.insert({"d55", [this](Image& image){ d55(image); }});

	all_build_methods_.insert({"hide", [this](Image& image){ hide(image); }});
	all_build_methods_.insert({"lhide", [this](Image& image){ lhide(image); }});
	all_build_methods_.insert({"rhide", [this](Image& image){ rhide(image); }});

	//all_build_methods_.insert({"focus", [this](Image& image){ autofocus_focus(image); }});
	//all_build_methods_.insert({"unfocus", [this](Image& image){ autofocus_unfocus(image); }});

	all_build_methods_.insert({"test", [this](Image& image){ test(image); }});

	all_build_methods_.insert({"none", nullptr}); //do nothing!

	/*t11(image);
	t21(image);
	t22(image);
	t31(image);
	t32(image);
	t33(image);
	t41(image);
	t42(image);
	t43(image);
	t44(image);
	t51(image);
	t52(image);
	t53(image);
	t54(image);
	t55(image);

	f11(image);
	f21(image);
	f22(image);
	f31(image);
	f32(image);
	f33(image);
	f41(image);
	f42(image);
	f43(image);
	f44(image);
	f51(image);
	f52(image);
	f53(image);
	f54(image);
	f55(image);

	h11(image);
	h21(image);
	h22(image);
	h31(image);
	h32(image);
	h33(image);
	h41(image);
	h42(image);
	h43(image);
	h44(image);
	h51(image);
	h52(image);
	h53(image);
	h54(image);
	h55(image);

	hf11(image);
	hf21(image);
	hf22(image);
	hf31(image);
	hf32(image);
	hf33(image);
	hf41(image);
	hf42(image);
	hf43(image);
	hf44(image);
	hf51(image);
	hf52(image);
	hf53(image);
	hf54(image);
	hf55(image);

	s11(image);
	s21(image);
	s22(image);
	s31(image);
	s32(image);
	s33(image);
	s41(image);
	s42(image);
	s43(image);
	s44(image);
	s51(image);
	s52(image);
	s53(image);
	s54(image);
	s55(image);

	d11(image);
	d21(image);
	d22(image);
	d31(image);
	d32(image);
	d33(image);
	d41(image);
	d42(image);
	d43(image);
	d44(image);
	d51(image);
	d52(image);
	d53(image);
	d54(image);
	d55(image);

	hide(image);
	lhide(image);
	rhide(image);*/

	/*all_transforms_.insert({"t11", [this](Image& image){ t11(image); }});
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

	all_transforms_.insert({"none", nullptr}); //do nothing!*/
}

//TODO : inutilisée
//Transform::Step Transform::create_step(Image& image, std::function<void(TransformStep&)> fonc)
//{
//	TransformStep transform_step(image);
//	auto fonc_without_param = [&fonc, &transform_step](){fonc(transform_step); };
//	return {transform_step, fonc_without_param};
//}

void Transform::add_step(Image& image, std::vector<LineOfTransformSteps>& transform_steps, std::function<void(TransformStep&)> fonc)
{
	TransformStep transform_step;
	//auto fonc_without_param = [fonc, transform_step]() mutable {fonc(transform_step); }; //Info : obligé de passer par copie car "fonc" et "transform_step" meurent à la fin de cette méthode
	transform_steps.back().push_back({transform_step, fonc});
}

void Transform::go_to_next_line_of_steps(std::vector<LineOfTransformSteps>& transform_steps)
{
	transform_steps.resize(transform_steps.size() + 1);
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
void Transform::recreate_transform(Image& image, std::string transform_name, bool is_visible)
{
	transform_name_ = transform_name;
	is_character_visible_ = is_visible; //to choose between on show and on replace
	is_finished_ = false;
	current_line_ = 0;
	//transform_ = {transform_name, TransformAllSteps(6)};

	fill_all_transforms();
	std::cout << this << std::endl;
	all_build_methods_.at(transform_name)(image);
}

void Transform::show_transform(std::string transform_name, Image& image)
{
	//std::cout << "CURRENT LINE: " << current_line_ << std::endl;
	if(all_transforms_.count(transform_name))
	{
		//all_transforms_[transform_name](image);

		if(!is_finished_)
		{
			std::vector<LineOfTransformSteps>& transform_steps = all_transforms_.at(transform_name);

			bool line_finished = true; //TODO : nom pas ouf

			std::cout << "TRANSFO: " << transform_name << ", " << current_line_ << ", " << transform_steps.size() << std::endl;

			for(Step& step : transform_steps[current_line_])
			{
				step.second(step.first);

				if(!step.first.is_finished_)
				{
					line_finished = false;
				}
			}
			if(line_finished)
			{
				if(transform_steps.size() - 1 > current_line_)
				{
					std::cout << "NEXT\n";
					current_line_ += 1;
				}
				else
				{
					std::cout << "TRANSFORM FINISHED!\n";
					is_finished_ = true;
				}
			}
		}
	}
	else
	{
		std::cerr << "TRANSFORMATION " << transform_name << " NOT FOUND...\n";
	}
}

std::vector<Transform::LineOfTransformSteps> Transform::tcommon(int xpos, Image& image)
{
	//Transform::TransformAllSteps& transform_t = transform_.second;

	//xpos est passé par référence alors qu'il meurt à la fin de cette méthode 
	//Façon 1 : [xpos, &image]
	//Façon 2 : passer directement les constantes de l'anonymous namespace

	std::vector<LineOfTransformSteps> transform_steps(1);
	if(is_character_visible_) //<=> on replace
	{
		add_step(image, transform_steps, [&](TransformStep& step){ step::reset(step, image); });
		add_step(image, transform_steps, [&](TransformStep& step){ step::set_alpha(step, image, 255); });
		add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, -26); });
		add_step(image, transform_steps, [&](TransformStep& step){ step::zoom(step, image, 0.8f); });
		add_step(image, transform_steps, [xpos, &image](TransformStep& step){ step::set_position_xcenter(step, image, xpos); });
	}
	else //<=> on show
	{
		add_step(image, transform_steps, [&](TransformStep& step){ step::reset(step, image); });
		add_step(image, transform_steps, [&](TransformStep& step){ step::hide(step, image); });
		add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, -26); });
		add_step(image, transform_steps, [&](TransformStep& step){ step::zoom(step, image, 0.8f); });
		add_step(image, transform_steps, [xpos, &image](TransformStep& step){ step::set_position_xcenter(step, image, xpos); });

		go_to_next_line_of_steps(transform_steps);

		add_step(image, transform_steps, [&](TransformStep& step){ step::show(step, image, Duration(250, Duration::Kind::LINEAR)); });
	}
	return transform_steps;


	//TODO : mettre ce if pour chaque transform ??
	/*if(!transform_t.transform_finished_)
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
	}*/
}

void Transform::t11(Image& image)
{
	all_transforms_.insert_or_assign("t11", tcommon(XPOS_11, image));
}

void Transform::t21(Image& image)
{
	all_transforms_.insert_or_assign("t21", tcommon(XPOS_21, image));
}

void Transform::t22(Image& image)
{
	all_transforms_.insert_or_assign("t22", tcommon(XPOS_22, image));
}

void Transform::t31(Image& image)
{
	all_transforms_.insert_or_assign("t31", tcommon(XPOS_31, image));
}

void Transform::t32(Image& image)
{
	all_transforms_.insert_or_assign("t32", tcommon(XPOS_32, image));
}

void Transform::t33(Image& image)
{
	all_transforms_.insert_or_assign("t33", tcommon(XPOS_33, image));
}

void Transform::t41(Image& image)
{
	all_transforms_.insert_or_assign("t41", tcommon(XPOS_41, image));
}

void Transform::t42(Image& image)
{
	all_transforms_.insert_or_assign("t42",tcommon(XPOS_42, image));
}

void Transform::t43(Image& image)
{
	all_transforms_.insert_or_assign("t43",tcommon(XPOS_43, image));
}

void Transform::t44(Image& image)
{
	all_transforms_.insert_or_assign("t44",tcommon(XPOS_44, image));
}

void Transform::t51(Image& image)
{
	all_transforms_.insert_or_assign("t51",tcommon(XPOS_51, image));
}

void Transform::t52(Image& image)
{
	all_transforms_.insert_or_assign("t52",tcommon(XPOS_52, image));
}

void Transform::t53(Image& image)
{
	all_transforms_.insert_or_assign("t53",tcommon(XPOS_53, image));
}

void Transform::t54(Image& image)
{
	all_transforms_.insert_or_assign("t54",tcommon(XPOS_54, image));
}

void Transform::t55(Image& image)
{
	all_transforms_.insert_or_assign("t55",tcommon(XPOS_55, image));
}

std::vector<Transform::LineOfTransformSteps> Transform::focus_common(int xpos, Image& image)
{
	//Transform::TransformAllSteps& transform_f = transform_.second;

	std::vector<LineOfTransformSteps> transform_steps(1);
	if(is_character_visible_) //<=> on replace
	{
		add_step(image, transform_steps, [&](TransformStep& step){ step::reset(step, image); });
		add_step(image, transform_steps, [&](TransformStep& step){ step::set_alpha(step, image, 255); });
		add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, -65); });
		add_step(image, transform_steps, [&](TransformStep& step){ step::zoom(step, image, 0.84f); });
		add_step(image, transform_steps, [xpos, &image](TransformStep& step){ step::set_position_xcenter(step, image, xpos); });
	}
	else //<=> on show
	{
		add_step(image, transform_steps, [&](TransformStep& step){ step::reset(step, image); });
		add_step(image, transform_steps, [&](TransformStep& step){ step::hide(step, image); });
		add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, -65); });
		add_step(image, transform_steps, [&](TransformStep& step){ step::zoom(step, image, 0.84f); });
		add_step(image, transform_steps, [xpos, &image](TransformStep& step){ step::set_position_xcenter(step, image, xpos); });

		go_to_next_line_of_steps(transform_steps);

		add_step(image, transform_steps, [&](TransformStep& step){ step::show(step, image, Duration(250, Duration::Kind::LINEAR)); });
	}
	return transform_steps;

	//TODO : mettre ce if pour chaque transform ??
	/*if(!transform_f.transform_finished_)
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
	}*/
}

void Transform::f11(Image& image)
{
	all_transforms_.insert_or_assign("f11", focus_common(XPOS_11, image));
}

void Transform::f21(Image& image)
{
	all_transforms_.insert_or_assign("f21", focus_common(XPOS_21, image));
}

void Transform::f22(Image& image)
{
	all_transforms_.insert_or_assign("f22", focus_common(XPOS_22, image));
}

void Transform::f31(Image& image)
{
	all_transforms_.insert_or_assign("f31", focus_common(XPOS_31, image));
}

void Transform::f32(Image& image)
{
	all_transforms_.insert_or_assign("f32", focus_common(XPOS_32, image));
}

void Transform::f33(Image& image)
{
	all_transforms_.insert_or_assign("f33", focus_common(XPOS_33, image));
}

void Transform::f41(Image& image)
{
	all_transforms_.insert_or_assign("f41", focus_common(XPOS_41, image));
}

void Transform::f42(Image& image)
{
	all_transforms_.insert_or_assign("f42", focus_common(XPOS_42, image));
}

void Transform::f43(Image& image)
{
	all_transforms_.insert_or_assign("f43", focus_common(XPOS_43, image));
}

void Transform::f44(Image& image)
{
	all_transforms_.insert_or_assign("f44", focus_common(XPOS_44, image));
}

void Transform::f51(Image& image)
{
	all_transforms_.insert_or_assign("f51", focus_common(XPOS_51, image));
}

void Transform::f52(Image& image)
{
	all_transforms_.insert_or_assign("f52", focus_common(XPOS_52, image));
}

void Transform::f53(Image& image)
{
	all_transforms_.insert_or_assign("f53", focus_common(XPOS_53, image));
}

void Transform::f54(Image& image)
{
	all_transforms_.insert_or_assign("f54", focus_common(XPOS_54, image));
}

void Transform::f55(Image& image)
{
	all_transforms_.insert_or_assign("f55", focus_common(XPOS_55, image));
}

void Transform::autofocus_focus(Image& image)
{
	//std::vector<LineOfTransformSteps> transform_steps(1);
	//if(is_character_visible_) //<=> on replace
	//{
	//	add_step(image, transform_steps, [&](TransformStep& step){ step::reset(step, image); });
	//	add_step(image, transform_steps, [&](TransformStep& step){ step::set_alpha(step, image, 255); });
	//	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, -65); });
	//	add_step(image, transform_steps, [&](TransformStep& step){ step::zoom(step, image, 0.84f); });
	//	add_step(image, transform_steps, [&image](TransformStep& step){ step::set_position_xcenter(step, image, image.position_.x); });
	//}
	//else //<=> on show
	//{
	//	add_step(image, transform_steps, [&](TransformStep& step){ step::reset(step, image); });
	//	add_step(image, transform_steps, [&](TransformStep& step){ step::hide(step, image); });
	//	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, -65); });
	//	add_step(image, transform_steps, [&](TransformStep& step){ step::zoom(step, image, 0.84f); });
	//	add_step(image, transform_steps, [&image](TransformStep& step){ step::set_position_xcenter(step, image, image.position_.x); });

	//	go_to_next_line_of_steps(transform_steps);

	//	add_step(image, transform_steps, [&](TransformStep& step){ step::show(step, image, Duration(250, Duration::Kind::LINEAR)); });
	//}
	//all_transforms_.insert_or_assign("focus", transform_steps);
}

void Transform::autofocus_unfocus(Image& image)
{
	//std::vector<LineOfTransformSteps> transform_steps(1);
	//if(is_character_visible_) //<=> on replace
	//{
	//	add_step(image, transform_steps, [&](TransformStep& step){ step::reset(step, image); });
	//	add_step(image, transform_steps, [&](TransformStep& step){ step::set_alpha(step, image, 255); });
	//	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, -26); });
	//	add_step(image, transform_steps, [&](TransformStep& step){ step::zoom(step, image, 0.8f); });
	//	add_step(image, transform_steps, [&image](TransformStep& step){ step::set_position_xcenter(step, image, image.position_.x); });
	//}
	//else //<=> on show
	//{
	//	add_step(image, transform_steps, [&](TransformStep& step){ step::reset(step, image); });
	//	add_step(image, transform_steps, [&](TransformStep& step){ step::hide(step, image); });
	//	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, -26); });
	//	add_step(image, transform_steps, [&](TransformStep& step){ step::zoom(step, image, 0.8f); });
	//	add_step(image, transform_steps, [&image](TransformStep& step){ step::set_position_xcenter(step, image, image.position_.x); });

	//	go_to_next_line_of_steps(transform_steps);

	//	add_step(image, transform_steps, [&](TransformStep& step){ step::show(step, image, Duration(250, Duration::Kind::LINEAR)); });
	//}
	//all_transforms_.insert_or_assign("unfocus", transform_steps);
}

std::vector<Transform::LineOfTransformSteps> Transform::hop(int xpos, Image& image)
{
	//Transform::TransformAllSteps& transform_h = transform_.second;

	std::vector<LineOfTransformSteps> transform_steps(1);

	add_step(image, transform_steps, [&](TransformStep& step){ step::reset(step, image); });
	add_step(image, transform_steps, [&](TransformStep& step){ step::zoom(step, image, 0.8f); });
	add_step(image, transform_steps, [xpos, &image](TransformStep& step){ step::set_position_xcenter(step, image, xpos); });
	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, -26); });

	go_to_next_line_of_steps(transform_steps);

	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, -20, Duration(100, Duration::Kind::EASEIN)); });

	go_to_next_line_of_steps(transform_steps);

	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, +20, Duration(100, Duration::Kind::EASEOUT)); });

	return transform_steps;

	//TODO : mettre ce if pour chaque transform ??
	/*if(!transform_h.transform_finished_)
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
	}*/
}

void Transform::h11(Image& image)
{
	all_transforms_.insert_or_assign("h11", hop(XPOS_11, image));
}

void Transform::h21(Image& image)
{
	all_transforms_.insert_or_assign("h21", hop(XPOS_21, image));
}

void Transform::h22(Image& image)
{
	all_transforms_.insert_or_assign("h22", hop(XPOS_22, image));
}

void Transform::h31(Image& image)
{
	all_transforms_.insert_or_assign("h31", hop(XPOS_31, image));
}

void Transform::h32(Image& image)
{
	all_transforms_.insert_or_assign("h32", hop(XPOS_32, image));
}

void Transform::h33(Image& image)
{
	all_transforms_.insert_or_assign("h33", hop(XPOS_33, image));
}

void Transform::h41(Image& image)
{
	all_transforms_.insert_or_assign("h41", hop(XPOS_41, image));
}

void Transform::h42(Image& image)
{
	all_transforms_.insert_or_assign("h42", hop(XPOS_42, image));
}

void Transform::h43(Image& image)
{
	all_transforms_.insert_or_assign("h43", hop(XPOS_43, image));
}

void Transform::h44(Image& image)
{
	all_transforms_.insert_or_assign("h44", hop(XPOS_44, image));
}

void Transform::h51(Image& image)
{
	all_transforms_.insert_or_assign("h51", hop(XPOS_51, image));
}

void Transform::h52(Image& image)
{
	all_transforms_.insert_or_assign("h52", hop(XPOS_52, image));
}

void Transform::h53(Image& image)
{
	all_transforms_.insert_or_assign("h53", hop(XPOS_53, image));
}

void Transform::h54(Image& image)
{
	all_transforms_.insert_or_assign("h54", hop(XPOS_54, image));
}

void Transform::h55(Image& image)
{
	all_transforms_.insert_or_assign("h55", hop(XPOS_55, image));
}

std::vector<Transform::LineOfTransformSteps> Transform::hop_focus(int xpos, Image& image)
{
	//Transform::TransformAllSteps& transform_hf = transform_.second;

	std::vector<LineOfTransformSteps> transform_steps(1);

	add_step(image, transform_steps, [&](TransformStep& step){ step::reset(step, image); });
	add_step(image, transform_steps, [&](TransformStep& step){ step::zoom(step, image, 0.84f); });
	add_step(image, transform_steps, [xpos, &image](TransformStep& step){ step::set_position_xcenter(step, image, xpos); });
	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, -65); });

	go_to_next_line_of_steps(transform_steps);

	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, -21, Duration(100, Duration::Kind::EASEIN)); });

	go_to_next_line_of_steps(transform_steps);

	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, +21, Duration(100, Duration::Kind::EASEOUT)); });

	return transform_steps;

	//TODO : mettre ce if pour chaque transform ??
	/*if(!transform_hf.transform_finished_)
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
	}*/
}

void Transform::hf11(Image& image)
{
	all_transforms_.insert_or_assign("hf11", hop_focus(XPOS_11, image));
}

void Transform::hf21(Image& image)
{
	all_transforms_.insert_or_assign("hf21", hop_focus(XPOS_21, image));
}

void Transform::hf22(Image& image)
{
	all_transforms_.insert_or_assign("hf22", hop_focus(XPOS_22, image));
}

void Transform::hf31(Image& image)
{
	all_transforms_.insert_or_assign("hf31", hop_focus(XPOS_31, image));
}

void Transform::hf32(Image& image)
{
	all_transforms_.insert_or_assign("hf32", hop_focus(XPOS_32, image));
}

void Transform::hf33(Image& image)
{
	all_transforms_.insert_or_assign("hf33", hop_focus(XPOS_33, image));
}

void Transform::hf41(Image& image)
{
	all_transforms_.insert_or_assign("hf41", hop_focus(XPOS_41, image));
}

void Transform::hf42(Image& image)
{
	all_transforms_.insert_or_assign("hf42", hop_focus(XPOS_42, image));
}

void Transform::hf43(Image& image)
{
	all_transforms_.insert_or_assign("hf43", hop_focus(XPOS_43, image));
}

void Transform::hf44(Image& image)
{
	all_transforms_.insert_or_assign("hf44", hop_focus(XPOS_44, image));
}

void Transform::hf51(Image& image)
{
	all_transforms_.insert_or_assign("hf51", hop_focus(XPOS_51, image));
}

void Transform::hf52(Image& image)
{
	all_transforms_.insert_or_assign("hf52", hop_focus(XPOS_52, image));
}

void Transform::hf53(Image& image)
{
	all_transforms_.insert_or_assign("hf53", hop_focus(XPOS_53, image));
}

void Transform::hf54(Image& image)
{
	all_transforms_.insert_or_assign("hf54", hop_focus(XPOS_54, image));
}

void Transform::hf55(Image& image)
{
	all_transforms_.insert_or_assign("hf55", hop_focus(XPOS_55, image));
}

std::vector<Transform::LineOfTransformSteps> Transform::sink(int xpos, Image& image)
{
	//Transform::TransformAllSteps& transform_s = transform_.second;

	std::vector<LineOfTransformSteps> transform_steps(1);

	add_step(image, transform_steps, [&](TransformStep& step){ step::reset(step, image); });
	add_step(image, transform_steps, [&](TransformStep& step){ step::zoom(step, image, 0.8f); });
	add_step(image, transform_steps, [xpos, &image](TransformStep& step){ step::set_position_xcenter(step, image, xpos); });
	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, -26); });

	go_to_next_line_of_steps(transform_steps);

	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, +20, Duration(500, Duration::Kind::EASEIN)); });

	return transform_steps;

	//TODO : mettre ce if pour chaque transform ??
	/*if(!transform_s.transform_finished_)
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
	}*/
}

void Transform::s11(Image& image)
{
	all_transforms_.insert_or_assign("s11", sink(XPOS_11, image));
}

void Transform::s21(Image& image)
{
	all_transforms_.insert_or_assign("s21", sink(XPOS_21, image));
}

void Transform::s22(Image& image)
{
	all_transforms_.insert_or_assign("s22", sink(XPOS_22, image));
}

void Transform::s31(Image& image)
{
	all_transforms_.insert_or_assign("s31", sink(XPOS_31, image));
}

void Transform::s32(Image& image)
{
	all_transforms_.insert_or_assign("s32", sink(XPOS_32, image));
}

void Transform::s33(Image& image)
{
	all_transforms_.insert_or_assign("s33", sink(XPOS_33, image));
}

void Transform::s41(Image& image)
{
	all_transforms_.insert_or_assign("s41", sink(XPOS_41, image));
}

void Transform::s42(Image& image)
{
	all_transforms_.insert_or_assign("s42", sink(XPOS_42, image));
}

void Transform::s43(Image& image)
{
	all_transforms_.insert_or_assign("s43", sink(XPOS_43, image));
}

void Transform::s44(Image& image)
{
	all_transforms_.insert_or_assign("s44", sink(XPOS_44, image));
}

void Transform::s51(Image& image)
{
	all_transforms_.insert_or_assign("s51", sink(XPOS_51, image));
}

void Transform::s52(Image& image)
{
	all_transforms_.insert_or_assign("s52", sink(XPOS_52, image));
}

void Transform::s53(Image& image)
{
	all_transforms_.insert_or_assign("s53", sink(XPOS_53, image));
}

void Transform::s54(Image& image)
{
	all_transforms_.insert_or_assign("s54", sink(XPOS_54, image));
}

void Transform::s55(Image& image)
{
	all_transforms_.insert_or_assign("s55", sink(XPOS_55, image));
}

std::vector<Transform::LineOfTransformSteps> Transform::dip(int xpos, Image& image)
{
	//Transform::TransformAllSteps& transform_d = transform_.second;

	std::vector<LineOfTransformSteps> transform_steps(1);

	add_step(image, transform_steps, [&](TransformStep& step){ step::reset(step, image); });
	add_step(image, transform_steps, [&](TransformStep& step){ step::zoom(step, image, 0.8f); });
	add_step(image, transform_steps, [xpos, &image](TransformStep& step){ step::set_position_xcenter(step, image, xpos); });
	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, -26); });

	go_to_next_line_of_steps(transform_steps);

	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, +25, Duration(250, Duration::Kind::EASEIN)); });

	go_to_next_line_of_steps(transform_steps);

	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_yoffset(step, image, -25, Duration(250, Duration::Kind::EASEOUT)); });

	return transform_steps;

	//TODO : mettre ce if pour chaque transform ??
	/*if(!transform_d.transform_finished_)
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
	}*/
}

void Transform::d11(Image& image)
{
	all_transforms_.insert_or_assign("d11", dip(XPOS_11, image));
}

void Transform::d21(Image& image)
{
	all_transforms_.insert_or_assign("d21", dip(XPOS_21, image));
}

void Transform::d22(Image& image)
{
	all_transforms_.insert_or_assign("d22", dip(XPOS_22, image));
}

void Transform::d31(Image& image)
{
	all_transforms_.insert_or_assign("d31", dip(XPOS_31, image));
}

void Transform::d32(Image& image)
{
	all_transforms_.insert_or_assign("d32", dip(XPOS_32, image));
}

void Transform::d33(Image& image)
{
	all_transforms_.insert_or_assign("d33", dip(XPOS_33, image));
}

void Transform::d41(Image& image)
{
	all_transforms_.insert_or_assign("d41", dip(XPOS_41, image));
}

void Transform::d42(Image& image)
{
	all_transforms_.insert_or_assign("d42", dip(XPOS_42, image));
}

void Transform::d43(Image& image)
{
	all_transforms_.insert_or_assign("d43", dip(XPOS_43, image));
}

void Transform::d44(Image& image)
{
	all_transforms_.insert_or_assign("d44", dip(XPOS_44, image));
}

void Transform::d51(Image& image)
{
	all_transforms_.insert_or_assign("d51", dip(XPOS_51, image));
}

void Transform::d52(Image& image)
{
	all_transforms_.insert_or_assign("d52", dip(XPOS_52, image));
}

void Transform::d53(Image& image)
{
	all_transforms_.insert_or_assign("d53", dip(XPOS_53, image));
}

void Transform::d54(Image& image)
{
	all_transforms_.insert_or_assign("d54", dip(XPOS_54, image));
}

void Transform::d55(Image& image)
{
	all_transforms_.insert_or_assign("d55", dip(XPOS_55, image));
}

void Transform::lhide(Image& image)
{
	//Transform::TransformAllSteps& transform_l = transform_.second;

	std::vector<LineOfTransformSteps> transform_steps(1);

	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_xcenter(step, image, -300, Duration(250, Duration::Kind::EASEOUT)); });

	all_transforms_.insert_or_assign("lhide", transform_steps);

	//TODO : mettre ce if pour chaque transform ??
	/*if(!transform_l.transform_finished_)
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
	}*/
}

void Transform::rhide(Image& image)
{
	//Transform::TransformAllSteps& transform_r = transform_.second;

	std::vector<LineOfTransformSteps> transform_steps(1);

	add_step(image, transform_steps, [&](TransformStep& step){ step::set_position_xcenter(step, image, +2000, Duration(250, Duration::Kind::EASEOUT)); });

	all_transforms_.insert_or_assign("rhide", transform_steps);

	//TODO : mettre ce if pour chaque transform ??
	/*if(!transform_r.transform_finished_)
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
	}*/
}

void Transform::test(Image& image) 
{
	//Transform::TransformAllSteps& transform_t = transform_.second;
	//switch(transform_t.current_step_number_)
	//{
	//	case 0:
	//		//transform_t.transform_steps_[0].zoom(image, 0.5f, 1000);
	//		//transform_t.transform_steps_[1].set_center(image, 3000);
	//		transform_t.transform_steps_[0].rotate(image, 360, Duration(3000, Duration::Kind::EASE));
	//		//transform_t.transform_steps_[3].set_alpha(image, 100, 3000);
	//		if(transform_t.transform_steps_[0].transform_step_finished_
	//		//&& transform_t.transform_steps_[1].transform_step_finished_
	//		/*&& transform_t.transform_steps_[2].transform_step_finished_
	//		&& transform_t.transform_steps_[3].transform_step_finished_*/)
	//		{
	//			transform_t.current_step_number_ += 1;
	//			transform_t.transform_finished_ = true;
	//		}
	//		break;

	//	default:
	//		break;
	//}
}

void Transform::hide(Image& image) //TODO : paramètre optionnel time
{
	//Transform::TransformAllSteps& transform_t = transform_.second;

	std::vector<LineOfTransformSteps> transform_steps(1);

	add_step(image, transform_steps, [&](TransformStep& step){ step::hide(step, image, Duration(250, Duration::Kind::LINEAR)); });

	all_transforms_.insert_or_assign("hide", transform_steps);

	/*switch(transform_t.current_step_number_)
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
	}*/
}