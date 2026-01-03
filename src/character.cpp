#include "character.h"
#include "transform.h"

#include <iostream>

//TODO : valeur par défaut du zorder = 0 ??

Character::Character(const CharacterDefinition& character_definition, sdl::Renderer& renderer, const std::string transform_name, const int zorder)
	: character_definition_(&character_definition)
	, properties_{character_definition.initial_name_, zorder, character_definition.initial_textbox_path_, character_definition.initial_namebox_path_, transform_name, false, false}, character_(character_definition.character_path_, 0, 0, renderer)
{}

void Character::set_transform(const std::string transform_name)
{
	if(properties_.transform_.transform_name_ != transform_name)
	{
		properties_.transform_.create_transform(transform_name, properties_.is_visible_);

		//TODO : si hide, mettre is_visible_ à false ??
		if(transform_name == "hide")
		{
			properties_.is_visible_ = false;
		}
		else
		{
			properties_.is_visible_ = true;
		}
	}
}


void Character::handle_events(const SDL_Event& e)
{
	(void)e;
}

void Character::draw(sdl::Renderer& renderer)
{
	if(properties_.is_visible_)
	{
		character_.draw(renderer);
	}
}

void Character::update()
{
	//std::cout << "Name: " << name_ << ", transfo: " << transform_.transform_name_ << std::endl;
	if(properties_.is_speaking_)
	{
		set_transform(properties_.transform_.transform_to_focus());
		//std::cout << properties_.name_ << " is speaking, " << properties_.transform_.transform_name_ << std::endl;
	}
	else
	{
		set_transform(properties_.transform_.transform_to_unfocus());
	}

	//std::cout << " transform: " << properties_.transform_.transform_name_ << std::endl;
	properties_.transform_.show_transform(properties_.transform_.transform_name_, character_);
}
