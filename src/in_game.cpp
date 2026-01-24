#include "in_game.h"
#include "game.h"

#include <iostream>

InGame::InGame(Game& game, sdl::Renderer& renderer)
	: GameState(game), script_runner_(game, renderer), renderer_(renderer)
{}

void InGame::handle_events(const SDL_Event& e)
{
	script_runner_.handle_events(e);
}

void InGame::draw(sdl::Renderer& renderer)
{
	script_runner_.draw(renderer);
}

void InGame::update()
{
	script_runner_.update();
}