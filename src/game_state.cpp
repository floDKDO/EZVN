#include "game_state.h"
#include "game.h"

GameState::GameState(Game& game)
	: game_(game), ui_manager_(game.audio_manager_)
{}

void GameState::handle_events(const SDL_Event& e)
{
	ui_manager_.handle_events(e);
}

void GameState::draw(sdl::Renderer& renderer)
{
	ui_manager_.draw(renderer);
}

void GameState::update()
{
	ui_manager_.update();
}