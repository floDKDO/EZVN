#include "character.h"
#include "transform.h"

#include <iostream>

//TODO : gérer les personnages sans image

Character::Character(const CharacterDefinition& character_definition, sdl::Renderer& renderer)
	: character_definition_(&character_definition)
	//TODO : si zorder, transform_name, is_speaking et is_visible ont toujours leur valeur par défaut, on peut retirer leur paramètre du constructeur de Character::Editableproperties
	, properties_{character_definition.initial_name_, constants::default_zorder_, character_definition.initial_textbox_path_, character_definition.initial_namebox_path_, "none", false, false}
{
	if(!character_definition_->character_path_.empty())
	{
		character_ = std::make_unique<Image>(character_definition.character_path_, 0, 0, renderer);
	}
}

void Character::set_transform(const std::string transform_name)
{
	if(transform_name == "none")
	{
		properties_.is_visible_ = true;
	}

	//std::cout << "Transfo: " << transform_name << ", " << properties_.transform_name_ << ", " << transform_.transform_name_ << std::endl;

	if(transform_.transform_name_ != transform_name)
	{
		transform_.create_transform(transform_name, properties_.is_visible_);

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
	if(character_ != nullptr && properties_.is_visible_)
	{
		character_->draw(renderer);
	}
}

void Character::update()
{
	if(character_ != nullptr)
	{
		//std::cout << "Name: " << name_ << ", transfo: " << transform_.transform_name_ << std::endl;
		if(properties_.is_speaking_)
		{
			set_transform(transform_.transform_to_focus());
			//std::cout << properties_.name_ << " is speaking, " << properties_.transform_.transform_name_ << std::endl;
		}
		else
		{
			set_transform(transform_.transform_to_unfocus());
		}

		//std::cout << " transform: " << properties_.transform_.transform_name_ << std::endl;
		transform_.show_transform(transform_.transform_name_, *character_);
	}
}
