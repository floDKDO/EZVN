#include "character.h"

Character::Character(std::string name, std::string character_path, std::string textbox_path, sdl::Renderer& renderer)
	: character_(character_path, 0, 0, renderer), textbox_(textbox_path, 0, 0, renderer) //TODO : pas besoin de textbox ?? //la position sera initialisée après
	, name_(name), is_speaking_(false)
{

}


void Character::set_transform(Transform t)
{
	//t.t11(character_.position_.x, character_.position_.y);
	character_.resize(character_.position_.w * 0.8, character_.position_.h * 0.8);
	character_.set_position(640, character_.position_.y);
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
