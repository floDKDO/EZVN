#include "character.h"
#include "transform.h"

#include <iostream>

Character::Character(std::string name, std::string character_path, std::string textbox_path, sdl::Renderer& renderer)
	: character_(character_path, 0, 0, renderer), textbox_(textbox_path, 0, 0, renderer) //TODO : pas besoin de textbox ?? //la position sera initialisée après
	, name_(name), is_speaking_(false), initial_rect_(character_.position_)
{

}


void Character::set_transform()
{
	t_.t11(character_, 0);
}


void Character::handle_events(const SDL_Event& e)
{
	(void)e;
}

void Character::draw(sdl::Renderer& renderer)
{
	character_.draw(renderer);
}

void Character::update(Uint64 time_step)
{
	(void)time_step;
}
