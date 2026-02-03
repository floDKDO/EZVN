#include "transform.h"
#include "image.h"

#include <iostream>
#include <regex>

//TODO : les transfos sur une même ligne ne fonctionnent que si elles ont toutes la même durée

//anonymous namespace
namespace
{

//TODO : l'anonymous namespace est encore pertinent ?
const std::unordered_map<std::string, int> xpos_suffixes_ =
{
	{"11", 640},

	{"21", 400},
	{"22", 880},

	{"31", 240},
	{"32", 640},
	{"33", 1040},

	{"41", 200},
	{"42", 493},
	{"43", 786},
	{"44", 1080},

	{"51", 140},
	{"52", 390},
	{"53", 640},
	{"54", 890},
	{"55", 1140}
};

//TODO : l'anonymous namespace est encore pertinent ?
const int get_xpos(std::string transform_name)
{
	std::cout << transform_name << std::endl;
	for(auto& [key, value] : xpos_suffixes_)
	{
		if(transform_name.find(key) != std::string::npos)
		{
			return value;
		}
	}
}

}

//Ce constructeur permet à Character::Editableproperties de disposer d'un constructeur par défaut
Transform::Transform()
	: Transform("none", 0, false)
{}

Transform::Transform(std::string transform_name, int number_of_transform_steps, bool is_character_visible)
	: is_character_visible_(is_character_visible) /*to choose between on show and on replace*/, transform_name_(transform_name), current_line_(0), is_finished_(false) 
{}

void Transform::fill_all_transforms(/*Image& image*/)
{
	if(all_build_methods_.size() > 0) //TODO : l'idéal serait de rendre all_build_methods_ static
	{
		return;
	}

	//TODO : regex => à la place de t11 -> t55, regex t[1-5][1-5] etc. 
	//std::regex t_transform("^t[1-9][1-9]$");
	//if(std::regex_match(string, t_transform));

	all_build_methods_.insert({"t11", [this](Image& image){ tcommon("t11", image); }});
	all_build_methods_.insert({"t21", [this](Image& image){ tcommon("t21", image); }});
	all_build_methods_.insert({"t22", [this](Image& image){ tcommon("t22", image); }});
	all_build_methods_.insert({"t31", [this](Image& image){ tcommon("t31", image); }});
	all_build_methods_.insert({"t32", [this](Image& image){ tcommon("t32", image); }});
	all_build_methods_.insert({"t33", [this](Image& image){ tcommon("t33", image); }});
	all_build_methods_.insert({"t41", [this](Image& image){ tcommon("t41", image); }});
	all_build_methods_.insert({"t42", [this](Image& image){ tcommon("t42", image); }});
	all_build_methods_.insert({"t43", [this](Image& image){ tcommon("t43", image); }});
	all_build_methods_.insert({"t44", [this](Image& image){ tcommon("t44", image); }});
	all_build_methods_.insert({"t51", [this](Image& image){ tcommon("t51", image); }});
	all_build_methods_.insert({"t52", [this](Image& image){ tcommon("t52", image); }});
	all_build_methods_.insert({"t53", [this](Image& image){ tcommon("t53", image); }});
	all_build_methods_.insert({"t54", [this](Image& image){ tcommon("t54", image); }});
	all_build_methods_.insert({"t55", [this](Image& image){ tcommon("t55", image); }});

	all_build_methods_.insert({"f11", [this](Image& image){ focus_common("f11", image); }});
	all_build_methods_.insert({"f21", [this](Image& image){ focus_common("f21", image); }});
	all_build_methods_.insert({"f22", [this](Image& image){ focus_common("f22", image); }});
	all_build_methods_.insert({"f31", [this](Image& image){ focus_common("f31", image); }});
	all_build_methods_.insert({"f32", [this](Image& image){ focus_common("f32", image); }});
	all_build_methods_.insert({"f33", [this](Image& image){ focus_common("f33", image); }});
	all_build_methods_.insert({"f41", [this](Image& image){ focus_common("f41", image); }});
	all_build_methods_.insert({"f42", [this](Image& image){ focus_common("f42", image); }});
	all_build_methods_.insert({"f43", [this](Image& image){ focus_common("f43", image); }});
	all_build_methods_.insert({"f44", [this](Image& image){ focus_common("f44", image); }});
	all_build_methods_.insert({"f51", [this](Image& image){ focus_common("f51", image); }});
	all_build_methods_.insert({"f52", [this](Image& image){ focus_common("f52", image); }});
	all_build_methods_.insert({"f53", [this](Image& image){ focus_common("f53", image); }});
	all_build_methods_.insert({"f54", [this](Image& image){ focus_common("f54", image); }});
	all_build_methods_.insert({"f55", [this](Image& image){ focus_common("f55", image); }});

	all_build_methods_.insert({"h11", [this](Image& image){ hop("h11", image); }});
	all_build_methods_.insert({"h21", [this](Image& image){ hop("h21", image); }});
	all_build_methods_.insert({"h22", [this](Image& image){ hop("h22", image); }});
	all_build_methods_.insert({"h31", [this](Image& image){ hop("h31", image); }});
	all_build_methods_.insert({"h32", [this](Image& image){ hop("h32", image); }});
	all_build_methods_.insert({"h33", [this](Image& image){ hop("h33", image); }});
	all_build_methods_.insert({"h41", [this](Image& image){ hop("h41", image); }});
	all_build_methods_.insert({"h42", [this](Image& image){ hop("h42", image); }});
	all_build_methods_.insert({"h43", [this](Image& image){ hop("h43", image); }});
	all_build_methods_.insert({"h44", [this](Image& image){ hop("h44", image); }});
	all_build_methods_.insert({"h51", [this](Image& image){ hop("h51", image); }});
	all_build_methods_.insert({"h52", [this](Image& image){ hop("h52", image); }});
	all_build_methods_.insert({"h53", [this](Image& image){ hop("h53", image); }});
	all_build_methods_.insert({"h54", [this](Image& image){ hop("h54", image); }});
	all_build_methods_.insert({"h55", [this](Image& image){ hop("h55", image); }});

	all_build_methods_.insert({"hf11", [this](Image& image){ hop_focus("hf11", image); }});
	all_build_methods_.insert({"hf21", [this](Image& image){ hop_focus("hf21", image); }});
	all_build_methods_.insert({"hf22", [this](Image& image){ hop_focus("hf22", image); }});
	all_build_methods_.insert({"hf31", [this](Image& image){ hop_focus("hf31", image); }});
	all_build_methods_.insert({"hf32", [this](Image& image){ hop_focus("hf32", image); }});
	all_build_methods_.insert({"hf33", [this](Image& image){ hop_focus("hf33", image); }});
	all_build_methods_.insert({"hf41", [this](Image& image){ hop_focus("hf41", image); }});
	all_build_methods_.insert({"hf42", [this](Image& image){ hop_focus("hf42", image); }});
	all_build_methods_.insert({"hf43", [this](Image& image){ hop_focus("hf43", image); }});
	all_build_methods_.insert({"hf44", [this](Image& image){ hop_focus("hf44", image); }});
	all_build_methods_.insert({"hf51", [this](Image& image){ hop_focus("hf51", image); }});
	all_build_methods_.insert({"hf52", [this](Image& image){ hop_focus("hf52", image); }});
	all_build_methods_.insert({"hf53", [this](Image& image){ hop_focus("hf53", image); }});
	all_build_methods_.insert({"hf54", [this](Image& image){ hop_focus("hf54", image); }});
	all_build_methods_.insert({"hf55", [this](Image& image){ hop_focus("hf55", image); }});

	all_build_methods_.insert({"s11", [this](Image& image){ sink("s11", image); }});
	all_build_methods_.insert({"s21", [this](Image& image){ sink("s21", image); }});
	all_build_methods_.insert({"s22", [this](Image& image){ sink("s22", image); }});
	all_build_methods_.insert({"s31", [this](Image& image){ sink("s31", image); }});
	all_build_methods_.insert({"s32", [this](Image& image){ sink("s32", image); }});
	all_build_methods_.insert({"s33", [this](Image& image){ sink("s33", image); }});
	all_build_methods_.insert({"s41", [this](Image& image){ sink("s41", image); }});
	all_build_methods_.insert({"s42", [this](Image& image){ sink("s42", image); }});
	all_build_methods_.insert({"s43", [this](Image& image){ sink("s43", image); }});
	all_build_methods_.insert({"s44", [this](Image& image){ sink("s44", image); }});
	all_build_methods_.insert({"s51", [this](Image& image){ sink("s51", image); }});
	all_build_methods_.insert({"s52", [this](Image& image){ sink("s52", image); }});
	all_build_methods_.insert({"s53", [this](Image& image){ sink("s53", image); }});
	all_build_methods_.insert({"s54", [this](Image& image){ sink("s54", image); }});
	all_build_methods_.insert({"s55", [this](Image& image){ sink("s55", image); }});

	all_build_methods_.insert({"d11", [this](Image& image){ dip("d11", image); }});
	all_build_methods_.insert({"d21", [this](Image& image){ dip("d21", image); }});
	all_build_methods_.insert({"d22", [this](Image& image){ dip("d22", image); }});
	all_build_methods_.insert({"d31", [this](Image& image){ dip("d31", image); }});
	all_build_methods_.insert({"d32", [this](Image& image){ dip("d32", image); }});
	all_build_methods_.insert({"d33", [this](Image& image){ dip("d33", image); }});
	all_build_methods_.insert({"d41", [this](Image& image){ dip("d41", image); }});
	all_build_methods_.insert({"d42", [this](Image& image){ dip("d42", image); }});
	all_build_methods_.insert({"d43", [this](Image& image){ dip("d43", image); }});
	all_build_methods_.insert({"d44", [this](Image& image){ dip("d44", image); }});
	all_build_methods_.insert({"d51", [this](Image& image){ dip("d51", image); }});
	all_build_methods_.insert({"d52", [this](Image& image){ dip("d52", image); }});
	all_build_methods_.insert({"d53", [this](Image& image){ dip("d53", image); }});
	all_build_methods_.insert({"d54", [this](Image& image){ dip("d54", image); }});
	all_build_methods_.insert({"d55", [this](Image& image){ dip("d55", image); }});

	all_build_methods_.insert({"hide", [this](Image& image){ hide(image); }});
	all_build_methods_.insert({"lhide", [this](Image& image){ lhide(image); }});
	all_build_methods_.insert({"rhide", [this](Image& image){ rhide(image); }});

	//all_build_methods_.insert({"focus", [this](Image& image){ autofocus_focus(image); }});
	//all_build_methods_.insert({"unfocus", [this](Image& image){ autofocus_unfocus(image); }});

	all_build_methods_.insert({"test", [this](Image& image){ test(image); }});

	all_build_methods_.insert({"none", nullptr}); //do nothing!
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

void Transform::recreate_transform(Image& image, std::string transform_name, bool is_visible)
{
	transform_name_ = transform_name;
	is_character_visible_ = is_visible; //to choose between on show and on replace

	is_finished_ = false;
	current_line_ = 0;

	fill_all_transforms();
	all_build_methods_.at(transform_name)(image);
}

void Transform::show_transform(std::string transform_name, Image& image)
{
	//std::cout << "CURRENT LINE: " << current_line_ << std::endl;
	if(all_transforms_.count(transform_name))
	{
		if(!is_finished_)
		{
			std::vector<LineOfTransformSteps>& transform_steps = all_transforms_.at(transform_name);

			bool line_finished = true; //TODO : nom pas ouf

			//std::cout << "TRANSFO: " << transform_name << ", " << current_line_ << ", " << transform_steps.size() << std::endl;

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
					//std::cout << "NEXT\n";
					current_line_ += 1;
				}
				else
				{
					//std::cout << "TRANSFORM FINISHED!\n";
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

Transform::LineOfTransformSteps Transform::add_line(std::vector<std::function<void(TransformStep&)>> foncs)
{
	LineOfTransformSteps line;
	for(const std::function<void(TransformStep&)>& fonc : foncs)
	{
		line.push_back({TransformStep(), fonc});
	}
	return line;
}

void Transform::tcommon(std::string transform_name, Image& image)
{
	std::vector<LineOfTransformSteps> transform_steps;
	if(is_character_visible_) //<=> on replace
	{
		transform_steps.push_back(
			add_line(
			{
				[&image](TransformStep& step){ step::reset(step, image); },
				[&image](TransformStep& step){ step::set_alpha(step, image, 255); },
				[&image](TransformStep& step){ step::set_position_yoffset(step, image, -26); },
				[&image](TransformStep& step){ step::zoom(step, image, 0.8f); },
				[&image, transform_name](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(transform_name)); }
			}));
	}
	else //<=> on show
	{
		transform_steps.push_back(
			add_line(
			{
				[&image](TransformStep& step){ step::reset(step, image); },
				[&image](TransformStep& step){ step::hide(step, image); },
				[&image](TransformStep& step){ step::set_position_yoffset(step, image, -26); },
				[&image](TransformStep& step){ step::zoom(step, image, 0.8f); },
				[&image, transform_name](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(transform_name)); }
			}));

		transform_steps.push_back(
			add_line(
			{
				[&image](TransformStep& step){ step::show(step, image, Duration(250, Duration::Kind::LINEAR)); }
			}));
	}
	all_transforms_.insert_or_assign(transform_name, transform_steps);
}

void Transform::focus_common(std::string transform_name, Image& image)
{
	//TODO : le bug disparaît quand la ligne du zoom est écrite en première, peu importe l'indice utilisé
	//=> n'est pas un "bug" mais le comportement normal. Il est logique que si j'applique d'abord set_center et ensuite un zoom, l'image ne sera plus centré mais l'ordre inverse, l'image sera centrée (cas time > 0)
	//=> Si je veux corriger ce problème, il faudrait que toutes les transfos avec time == 0 soit appliquée en même temps et non pas ligne par ligne

	std::vector<LineOfTransformSteps> transform_steps;
	if(is_character_visible_) //<=> on replace
	{
		transform_steps.push_back(
			add_line(
			{
				[&image](TransformStep& step){ step::reset(step, image); },
				[&image](TransformStep& step){ step::set_alpha(step, image, 255); },
				[&image](TransformStep& step){ step::set_position_yoffset(step, image, -65); },
				[&image](TransformStep& step){ step::zoom(step, image, 0.84f); },
				[&image, transform_name](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(transform_name)); }
			}));
	}
	else //<=> on show
	{
		transform_steps.push_back(
			add_line(
			{
				[&image](TransformStep& step){ step::reset(step, image); },
				[&image](TransformStep& step){ step::hide(step, image); },
				[&image](TransformStep& step){ step::set_position_yoffset(step, image, -65); },
				[&image](TransformStep& step){ step::zoom(step, image, 0.84f); },
				[&image, transform_name](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(transform_name)); }
			}));

		transform_steps.push_back(
			add_line(
			{
				[&image](TransformStep& step){ step::show(step, image, Duration(250, Duration::Kind::LINEAR)); }
			}));
	}
	all_transforms_.insert_or_assign(transform_name, transform_steps);
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

void Transform::hop(std::string transform_name, Image& image)
{
	std::vector<LineOfTransformSteps> transform_steps;

	transform_steps.push_back(
		add_line(
		{
			[&image](TransformStep& step){ step::reset(step, image); },
			[&image](TransformStep& step){ step::zoom(step, image, 0.8f); },
			[&image, transform_name](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(transform_name)); },
			[&image](TransformStep& step){ step::set_position_yoffset(step, image, -26); }
		}));

	transform_steps.push_back(
		add_line(
		{
			[&image](TransformStep& step){ step::set_position_yoffset(step, image, -20, Duration(100, Duration::Kind::EASEIN)); }
		}));

	transform_steps.push_back(
		add_line(
		{
			[&image](TransformStep& step){ step::set_position_yoffset(step, image, +20, Duration(100, Duration::Kind::EASEOUT)); }
		}));

	all_transforms_.insert_or_assign(transform_name, transform_steps);
}

void Transform::hop_focus(std::string transform_name, Image& image)
{
	std::vector<LineOfTransformSteps> transform_steps;

	transform_steps.push_back(
		add_line(
		{
			[&image](TransformStep& step){ step::reset(step, image); },
			[&image](TransformStep& step){ step::zoom(step, image, 0.84f); },
			[&image, transform_name](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(transform_name)); },
			[&image](TransformStep& step){ step::set_position_yoffset(step, image, -65); }
		}));

	transform_steps.push_back(
		add_line(
		{
			[&image](TransformStep& step){ step::set_position_yoffset(step, image, -21, Duration(100, Duration::Kind::EASEIN)); }
		}));

	transform_steps.push_back(
		add_line(
		{
			[&image](TransformStep& step){ step::set_position_yoffset(step, image, +21, Duration(100, Duration::Kind::EASEOUT)); }
		}));

	all_transforms_.insert_or_assign(transform_name, transform_steps);
}

void Transform::sink(std::string transform_name, Image& image)
{
	std::vector<LineOfTransformSteps> transform_steps;

	transform_steps.push_back(
		add_line(
		{
			[&image](TransformStep& step){ step::reset(step, image); },
			[&image](TransformStep& step){ step::zoom(step, image, 0.8f); },
			[&image, transform_name](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(transform_name)); },
			[&image](TransformStep& step){ step::set_position_yoffset(step, image, -26); }
		}));

	transform_steps.push_back(
		add_line(
		{
			[&image](TransformStep& step){ step::set_position_yoffset(step, image, +20, Duration(500, Duration::Kind::EASEIN)); }
		}));

	all_transforms_.insert_or_assign(transform_name, transform_steps);
}

void Transform::dip(std::string transform_name, Image& image)
{
	std::vector<LineOfTransformSteps> transform_steps;

	transform_steps.push_back(
		add_line(
		{
			[&image](TransformStep& step){ step::reset(step, image); },
			[&image](TransformStep& step){ step::zoom(step, image, 0.8f); },
			[&image, transform_name](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(transform_name)); },
			[&image](TransformStep& step){ step::set_position_yoffset(step, image, -26); }
		}));

	transform_steps.push_back(
		add_line(
		{
			[&image](TransformStep& step){ step::set_position_yoffset(step, image, +25, Duration(250, Duration::Kind::EASEIN)); }
		}));

	transform_steps.push_back(
		add_line(
		{
			[&image](TransformStep& step){ step::set_position_yoffset(step, image, -25, Duration(250, Duration::Kind::EASEOUT)); }
		}));

	all_transforms_.insert_or_assign(transform_name, transform_steps);
}

void Transform::lhide(Image& image)
{
	std::vector<LineOfTransformSteps> transform_steps;

	//TODO : inverser le sprite quand il va à gauche

	transform_steps.push_back(
		add_line(
		{
			[&image](TransformStep& step){ step::set_position_xcenter(step, image, -300, Duration(250, Duration::Kind::EASEOUT)); }
		}));

	all_transforms_.insert_or_assign("lhide", transform_steps);
}

void Transform::rhide(Image& image)
{
	std::vector<LineOfTransformSteps> transform_steps;

	transform_steps.push_back(
		add_line(
		{
			[&image](TransformStep& step){ step::set_position_xcenter(step, image, +2000, Duration(250, Duration::Kind::EASEOUT)); }
		}));

	all_transforms_.insert_or_assign("rhide", transform_steps);
}

void Transform::test(Image& image) 
{

}

void Transform::hide(Image& image) //TODO : paramètre optionnel time
{
	std::vector<LineOfTransformSteps> transform_steps;

	transform_steps.push_back(
		add_line(
		{
			[&image](TransformStep& step){ step::hide(step, image, Duration(250, Duration::Kind::LINEAR)); }
		}));

	all_transforms_.insert_or_assign("hide", transform_steps);
}