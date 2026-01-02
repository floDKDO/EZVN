#include "character.h"
#include "transform.h"

#include <iostream>

//TODO : valeur par défaut du zorder = 0 ??

Character::Character(const CharacterDefinition& character_definition, sdl::Renderer& renderer, const std::string transform_name, const int zorder)
	: character_definition_(&character_definition)
	, properties_{character_definition.initial_name_, zorder, character_definition.initial_textbox_path_, character_definition.initial_namebox_path_, Transform(transform_name), false, false}, character_(character_definition.character_path_, 0, 0, renderer)
{
	std::cout << "CONSTRUCTEUR: " << &character_definition << std::endl;
}

/*Character::Character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, sdl::Renderer& renderer)
	: character_definition_(character_variable, character_name, character_path), character_(character_path, 0, 0, renderer)
	, properties_{std::string(character_name), 0, "", "", Transform(""), false, false}
	//, initial_rect_(character_.position_)
{}

Character::Character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, sdl::Renderer& renderer, const SDL_Color namebox_font_color, const std::string_view textbox_path, const std::string_view namebox_path)
	: character_definition_(character_variable, character_name, character_path, namebox_font_color, textbox_path, namebox_path), character_(character_path, 0, 0, renderer)
	, properties_{std::string(character_name), 0, std::string(textbox_path), std::string(namebox_path), Transform(""), false, false}
	//, initial_rect_(character_.position_)
{}*/

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
