#include "character.h"
#include "transform.h"

#include <iostream>

//TODO : gérer les personnages sans image

Character::Character(const CharacterDefinition& character_definition, sdl::Renderer& renderer)
	: character_definition_(&character_definition)
	, properties_{character_definition.initial_name_, character_definition.initial_namebox_text_color_, character_definition.initial_textbox_path_, character_definition.initial_namebox_path_}
{
	if(!character_definition_->character_path_.empty())
	{
		character_ = std::make_unique<Image>(character_definition.character_path_, 0, 0, renderer);
	}
}

void Character::set_transform(std::string transform_name)
{
	if(transform_name == "none")
	{
		properties_.is_visible_ = true;
	}

	//std::cout << "Transfo: " << transform_name << ", " << properties_.transform_name_ << ", " << transform_.transform_name_ << ", " << std::boolalpha << properties_.is_visible_ << std::endl;

	if(transform_.transform_name_ != transform_name)
	{
		if(character_ != nullptr)
		{
			transform_.recreate_transform(*character_, transform_name, properties_.is_visible_);
		}

		//TODO : si hide, mettre is_visible_ à false ?? => ne change rien car hide le personnage le supprime de active_characters_ (plus maintenant, voir ligne 84 dans character_manager.cpp)
		if(transform_name == "hide")
		{
			properties_.transform_name_ = "hide";
			properties_.is_visible_ = false;
		}
		else
		{
			properties_.is_visible_ = true;
		}
	}
}

void Character::draw(sdl::Renderer& renderer)
{
	if(character_ != nullptr && properties_.is_visible_)
	{
		character_->draw(renderer);
	}
}

void Character::update()
{
	if(character_ != nullptr)
	{
		//std::cout << "Name: " << character_definition_->character_variable_ << ", transfo: " << transform_.transform_name_ << ", is_visible_: " << std::boolalpha << properties_.is_visible_ << std::endl;
		if(properties_.is_speaking_)
		{
			set_transform(transform_.transform_to_focus());
			//std::cout << properties_.name_ << " is speaking, " << properties_.transform_name_ << std::endl;
		}
		else
		{
			set_transform(transform_.transform_to_unfocus());
		}

		//std::cout << " transform: " << properties_.transform_name_ << std::endl;
		transform_.show_transform(transform_.transform_name_, *character_);

		character_->update();
	}
}

bool Character::has_image()
{
	return character_ != nullptr;
}
