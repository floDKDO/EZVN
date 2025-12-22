#include "character.h"
#include "transform.h"

#include <iostream>

int Character::counter_ = 0;

//TODO : valeur par défaut du zorder = 0 ??

Character::Character(const std::string_view character_name, const std::string_view character_path, sdl::Renderer& renderer)
	: character_(character_path, 0, 0, renderer)
	, properties_{std::string(character_name), 0, "", "", Transform("")}
	, unique_id_(std::string(character_name) + std::to_string(counter_)), is_speaking_(false), is_visible_(false) //, initial_rect_(character_.position_)
{
	counter_ += 1;
}

Character::Character(const std::string_view character_name, const std::string_view character_path, sdl::Renderer& renderer, const std::string_view textbox_path, const std::string_view namebox_path)
	: character_(character_path, 0, 0, renderer)
	, properties_{std::string(character_name), 0, std::string(textbox_path), std::string(namebox_path), Transform("")}
	, unique_id_(std::string(character_name) + std::to_string(counter_)), is_speaking_(false), is_visible_(false) //, initial_rect_(character_.position_)
{
	counter_ += 1;
}

void Character::set_transform(const std::string transform_name)
{
	if(properties_.transform_.transform_name_ != transform_name)
	{
		properties_.transform_.create_transform(transform_name, is_visible_);

		//TODO : si hide, mettre is_visible_ à false ??
		if(transform_name == "hide")
		{
			is_visible_ = false;
		}
		else
		{
			is_visible_ = true;
		}
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
