#include "character.h"
#include "transform.h"

#include <iostream>

int Character::counter_ = 0;

Character::Character(const std::string_view name, const std::string_view character_path, sdl::Renderer& renderer, const int zorder)
	: character_(character_path, 0, 0, renderer, zorder)
	, properties_{std::string(name), zorder, "", "", Transform("")}
	, unique_id_(std::string(name) + std::to_string(counter_)), is_speaking_(false), is_visible_(false) //, initial_rect_(character_.position_)
{
	counter_ += 1;
}

Character::Character(const std::string_view name, const std::string_view character_path, sdl::Renderer& renderer, const std::string_view textbox_path, const std::string_view namebox_path, const int zorder)
	: character_(character_path, 0, 0, renderer, zorder)
	, properties_{std::string(name), zorder, std::string(textbox_path), std::string(namebox_path), Transform("")}
	, unique_id_(std::string(name) + std::to_string(counter_)), is_speaking_(false), is_visible_(false) //, initial_rect_(character_.position_)
{
	counter_ += 1;
}

void Character::set_transform(const std::string transform_name)
{
	//std::cout << transform_.transform_name_ << " et " << transform_name << std::endl;
	//if(transform_name == "none")
	//{
	//	is_visible_ = true;
	//}

	if(properties_.transform_.transform_name_ != transform_name)
	{
		properties_.transform_.create_transform(transform_name, is_visible_);
		//transform_.transform_name_ = transform_name;
		//std::cout << int(transform_.transform_name_) << std::endl;

		//TODO : si hide, mettre is_visible_ à false ??
		if(transform_name == "hide")
		{
			is_visible_ = false;
		}
		else
		{
			is_visible_ = true;
		}
		//std::cout << std::boolalpha << is_visible_ << std::endl;

		//transform_.show_transform(transform_name, character_);
	}
}


void Character::handle_events(const SDL_Event& e)
{
	(void)e;
}

void Character::draw(sdl::Renderer& renderer)
{
	if(is_visible_)
	{
		character_.draw(renderer);
	}
}

void Character::update()
{
	//std::cout << "Name: " << name_ << ", transfo: " << transform_.transform_name_ << std::endl;
	if(is_speaking_)
	{
		set_transform(properties_.transform_.transform_to_focus());
	}
	else
	{
		set_transform(properties_.transform_.transform_to_unfocus());
	}

	properties_.transform_.show_transform(properties_.transform_.transform_name_, character_);
}
