#include "character.h"
#include "transform.h"

#include <iostream>

Character::Character(const std::string_view name, const std::string_view character_path, sdl::Renderer& renderer, const int zorder)
	: character_(character_path, 0, 0, renderer, zorder)
	, name_(name), is_speaking_(false), is_visible_(false), t_(TransformName::none)//, initial_rect_(character_.position_)
{}

void Character::set_transform(const TransformName transform_name)
{
	if(t_.transform_name_ != transform_name)
	{
		t_.create_transform(transform_name);
		//t_.transform_name_ = transform_name;
		//std::cout << int(t_.transform_name_) << std::endl;

		//TODO : si hide, mettre is_visible_ à false ??
		is_visible_ = true;

		//t_.show_transform(transform_name, character_);
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

}
