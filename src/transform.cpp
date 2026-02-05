#include "transform.h"
#include "image.h"

#include <regex>
#include <iostream>

//TODO : les transfos sur une même ligne ne fonctionnent que si elles ont toutes la même durée

//anonymous namespace
namespace
{

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

const int get_xpos(std::string_view transform_name)
{
	for(auto& [key, value] : xpos_suffixes_)
	{
		if(transform_name.find(key) != std::string_view::npos)
		{
			return value;
		}
	}
	return 0;
}

}

//Ce constructeur permet à Character::Editableproperties de disposer d'un constructeur par défaut
Transform::Transform()
	: Transform("none", false)
{}

Transform::Transform(std::string_view transform_name, bool is_character_visible)
	: is_character_visible_(is_character_visible), transform_name_(transform_name), previous_transform_name_("none")
{}

void Transform::init_transform(Image& image, std::string transform_name)
{
	if(std::regex_match(transform_name, std::regex("^t[1-5][1-5]$")))
	{
		all_build_methods_.insert({transform_name, [this, transform_name](Image& image){ tcommon(transform_name, image); }});
	}
	else if(std::regex_match(transform_name, std::regex("^f[1-5][1-5]$")))
	{
		all_build_methods_.insert({transform_name, [this, transform_name](Image& image){ focus_common(transform_name, image); }});
	}
	else if(std::regex_match(transform_name, std::regex("^h[1-5][1-5]$")))
	{
		all_build_methods_.insert({transform_name, [this, transform_name](Image& image){ hop(transform_name, image); }});
	}
	else if(std::regex_match(transform_name, std::regex("^hf[1-5][1-5]$")))
	{
		all_build_methods_.insert({transform_name, [this, transform_name](Image& image){ hop_focus(transform_name, image); }});
	}
	else if(std::regex_match(transform_name, std::regex("^s[1-5][1-5]$")))
	{
		all_build_methods_.insert({transform_name, [this, transform_name](Image& image){ sink(transform_name, image); }});
	}
	else if(std::regex_match(transform_name, std::regex("^d[1-5][1-5]$")))
	{
		all_build_methods_.insert({transform_name, [this, transform_name](Image& image){ dip(transform_name, image); }});
	}
	else if(transform_name == "hide")
	{
		all_build_methods_.insert({transform_name, [this](Image& image){ hide(image); }});
	}
	else if(transform_name == "lhide")
	{
		all_build_methods_.insert({transform_name, [this](Image& image){ lhide(image); }});
	}
	else if(transform_name == "rhide")
	{
		all_build_methods_.insert({transform_name, [this](Image& image){ rhide(image); }});
	}
	else if(transform_name == "test")
	{
		all_build_methods_.insert({transform_name, [this](Image& image){ test(image); }});
	}
	else if(transform_name == "focus")
	{
		all_build_methods_.insert({transform_name, [this](Image& image){ focus(image); }});
	}
	else if(transform_name == "unfocus")
	{
		all_build_methods_.insert({transform_name, [this](Image& image){ unfocus(image); }});
	}
	else if(transform_name == "none")
	{
		all_build_methods_.insert({transform_name, nullptr}); //do nothing!
	}
	all_build_methods_.at(transform_name)(image);
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
	previous_transform_name_ = transform_name_;
	transform_name_ = transform_name;
	is_character_visible_ = is_visible; //to choose between "on show" and "on replace" 
	init_transform(image, transform_name);
}

void Transform::show_transform(std::string_view transform_name)
{
	const std::string& str_transform_name = std::string(transform_name);
	if(all_transforms_.count(str_transform_name))
	{
		AllLinesOfSteps& lines = all_transforms_.at(str_transform_name);
		if(!lines.is_finished_)
		{
			LineOfSteps& line = lines.get_current_line();

			for(Step& step : line.steps_)
			{
				if(!step.transform_step_.is_finished_)
				{
					step.fonc_(step.transform_step_);
				}
			}

			if(line.is_line_finished())
			{
				lines.go_to_next_line();
			}
		}
	}
	else
	{
		std::cerr << "TRANSFORM " << transform_name << " NOT FOUND...\n";
	}
}

void Transform::tcommon(std::string transform_name, Image& image)
{
	AllLinesOfSteps lines;

	if(is_character_visible_) //<=> on replace
	{
		lines.add_line({
			[&image](TransformStep& step){ step::reset(step, image); },
			[&image](TransformStep& step){ step::set_alpha(step, image, 255); },
			[&image](TransformStep& step){ step::set_position_yoffset(step, image, -26); },
			[&image](TransformStep& step){ step::zoom(step, image, 0.8f); },
			[&image, transform_name](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(transform_name)); }
		});
	}
	else //<=> on show
	{
		lines.add_line({
			[&image](TransformStep& step){ step::reset(step, image); },
			[&image](TransformStep& step){ step::hide(step, image); },
			[&image](TransformStep& step){ step::set_position_yoffset(step, image, -26); },
			[&image](TransformStep& step){ step::zoom(step, image, 0.8f); },
			[&image, transform_name](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(transform_name)); }
		});

		lines.add_line({
			[&image](TransformStep& step){ step::show(step, image, Duration(250, Duration::Kind::LINEAR)); }
		});
	}
	all_transforms_.insert_or_assign(transform_name, lines);
}

void Transform::focus_common(std::string transform_name, Image& image)
{
	//TODO : le bug disparaît quand la ligne du zoom est écrite en première, peu importe l'indice utilisé
	//=> n'est pas un "bug" mais le comportement normal. Il est logique que si j'applique d'abord set_center et ensuite un zoom, l'image ne sera plus centré mais l'ordre inverse, l'image sera centrée (cas time > 0)
	//=> Si je veux corriger ce problème, il faudrait que toutes les transfos avec time == 0 soit appliquée en même temps et non pas ligne par ligne

	AllLinesOfSteps lines;
	if(is_character_visible_) //<=> on replace
	{
		lines.add_line({
			[&image](TransformStep& step){ step::reset(step, image); },
			[&image](TransformStep& step){ step::set_alpha(step, image, 255); },
			[&image](TransformStep& step){ step::set_position_yoffset(step, image, -65); },
			[&image](TransformStep& step){ step::zoom(step, image, 0.84f); },
			[&image, transform_name](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(transform_name)); }
		});
	}
	else //<=> on show
	{
		lines.add_line({
			[&image](TransformStep& step){ step::reset(step, image); },
			[&image](TransformStep& step){ step::hide(step, image); },
			[&image](TransformStep& step){ step::set_position_yoffset(step, image, -65); },
			[&image](TransformStep& step){ step::zoom(step, image, 0.84f); },
			[&image, transform_name](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(transform_name)); }
		});

		lines.add_line({
			[&image](TransformStep& step){ step::show(step, image, Duration(250, Duration::Kind::LINEAR)); }
		});
	}
	all_transforms_.insert_or_assign(transform_name, lines);
}

void Transform::hop(std::string transform_name, Image& image)
{
	AllLinesOfSteps lines;

	lines.add_line({
		[&image](TransformStep& step){ step::reset(step, image); },
		[&image](TransformStep& step){ step::zoom(step, image, 0.8f); },
		[&image, transform_name](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(transform_name)); },
		[&image](TransformStep& step){ step::set_position_yoffset(step, image, -26); }
	});

	lines.add_line({
		[&image](TransformStep& step){ step::set_position_yoffset(step, image, -20, Duration(100, Duration::Kind::EASEIN)); }
	});

	lines.add_line({
		[&image](TransformStep& step){ step::set_position_yoffset(step, image, +20, Duration(100, Duration::Kind::EASEOUT)); }
	});

	all_transforms_.insert_or_assign(transform_name, lines);
}

void Transform::hop_focus(std::string transform_name, Image& image)
{
	AllLinesOfSteps lines;

	lines.add_line({
		[&image](TransformStep& step){ step::reset(step, image); },
		[&image](TransformStep& step){ step::zoom(step, image, 0.84f); },
		[&image, transform_name](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(transform_name)); },
		[&image](TransformStep& step){ step::set_position_yoffset(step, image, -65); }
	});

	lines.add_line({
		[&image](TransformStep& step){ step::set_position_yoffset(step, image, -21, Duration(100, Duration::Kind::EASEIN)); }
	});

	lines.add_line({
		[&image](TransformStep& step){ step::set_position_yoffset(step, image, +21, Duration(100, Duration::Kind::EASEOUT)); }
	});

	all_transforms_.insert_or_assign(transform_name, lines);
}

void Transform::sink(std::string transform_name, Image& image)
{
	AllLinesOfSteps lines;

	lines.add_line({
		[&image](TransformStep& step){ step::reset(step, image); },
		[&image](TransformStep& step){ step::zoom(step, image, 0.8f); },
		[&image, transform_name](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(transform_name)); },
		[&image](TransformStep& step){ step::set_position_yoffset(step, image, -26); }
	});

	lines.add_line({
		[&image](TransformStep& step){ step::set_position_yoffset(step, image, +20, Duration(500, Duration::Kind::EASEIN)); }
	});

	all_transforms_.insert_or_assign(transform_name, lines);
}

void Transform::dip(std::string transform_name, Image& image)
{
	AllLinesOfSteps lines;

	lines.add_line({
		[&image](TransformStep& step){ step::reset(step, image); },
		[&image](TransformStep& step){ step::zoom(step, image, 0.8f); },
		[&image, transform_name](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(transform_name)); },
		[&image](TransformStep& step){ step::set_position_yoffset(step, image, -26); }
	});

	lines.add_line({
		[&image](TransformStep& step){ step::set_position_yoffset(step, image, +25, Duration(250, Duration::Kind::EASEIN)); }
	});

	lines.add_line({
		[&image](TransformStep& step){ step::set_position_yoffset(step, image, -25, Duration(250, Duration::Kind::EASEOUT)); }
	});

	all_transforms_.insert_or_assign(transform_name, lines);
}

void Transform::lhide(Image& image)
{
	AllLinesOfSteps lines;

	//TODO : inverser le sprite quand il va à gauche

	lines.add_line({
		[&image](TransformStep& step){ step::set_position_xcenter(step, image, -300, Duration(250, Duration::Kind::EASEOUT)); }
	});

	all_transforms_.insert_or_assign("lhide", lines);
}

void Transform::rhide(Image& image)
{
	AllLinesOfSteps lines;

	//TODO : inverser le sprite quand il va à droite

	lines.add_line({
		[&image](TransformStep& step){ step::set_position_xcenter(step, image, +2000, Duration(250, Duration::Kind::EASEOUT)); }
	});

	all_transforms_.insert_or_assign("rhide", lines);
}

void Transform::test(Image& image) 
{
	AllLinesOfSteps lines;

	lines.add_line({
		[&image](TransformStep& step){ step::rotate(step, image, 360.0, Duration(250, Duration::Kind::EASEOUT)); }
	});

	lines.add_line({
		[&image](TransformStep& step){ step::set_center(step, image, Duration(2500, Duration::Kind::EASEOUT)); }
	});

	all_transforms_.insert_or_assign("test", lines);
}

void Transform::hide(Image& image) //TODO : paramètre optionnel time  => pas un paramètre mais une variable globale / membre
{
	AllLinesOfSteps lines;

	lines.add_line({
		[&image](TransformStep& step){ step::hide(step, image, Duration(250, Duration::Kind::LINEAR)); }
	});

	all_transforms_.insert_or_assign("hide", lines);
}

void Transform::focus(Image& image)
{
	AllLinesOfSteps lines;

	std::cout << "FOCUS CALLED\n";

	lines.add_line({
		[&image](TransformStep& step){ step::zoom(step, image, 0.84f, Duration(0, Duration::Kind::LINEAR)); },
		[&image](TransformStep& step){ step::set_position_yoffset(step, image, -65); },
		[this, &image](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(previous_transform_name_)); }
	});

	all_transforms_.insert_or_assign("focus", lines);
}

void Transform::unfocus(Image& image)
{
	AllLinesOfSteps lines;

	std::cout << "UNFOCUS CALLED\n";

	lines.add_line({
		[&image](TransformStep& step){ step::zoom(step, image, 0.8f, Duration(0, Duration::Kind::LINEAR)); },
		[&image](TransformStep& step){ step::set_position_yoffset(step, image, -26); },
		[this, &image](TransformStep& step){ step::set_position_xcenter(step, image, get_xpos(previous_transform_name_)); }
	});

	all_transforms_.insert_or_assign("unfocus", lines);
}