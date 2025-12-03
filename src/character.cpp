#include "character.h"
#include "transform.h"

#include <iostream>

Character::Character(const std::string name, const std::string character_path, sdl::Renderer& renderer, const int zorder)
	: character_(character_path, 0, 0, renderer, zorder)
	, name_(name), is_speaking_(false), initial_rect_(character_.position_)
{

}

void Character::set_transform(const TransformName transform_name)
{
	t_.show_transform(transform_name, character_);
}


void Character::handle_events(const SDL_Event& e)
{
	(void)e;
}

void Character::draw(sdl::Renderer& renderer)
{
	character_.draw(renderer);
}

void Character::update()
{

}
