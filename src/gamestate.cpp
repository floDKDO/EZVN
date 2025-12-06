#include "gamestate.h"

GameState::GameState(Game& game)
	: game_(game)
{}

void GameState::handle_events(const SDL_Event& e)
{
	for(const std::unique_ptr<Ui>& ui : ui_elements_)
	{
		ui->handle_events(e);
	}
}

void GameState::draw(sdl::Renderer& renderer)
{
	for(const std::unique_ptr<Ui>& ui : ui_elements_)
	{
		ui->draw(renderer);
	}
}

void GameState::update()
{
	for(const std::unique_ptr<Ui>& ui : ui_elements_)
	{
		ui->update();
	}
}