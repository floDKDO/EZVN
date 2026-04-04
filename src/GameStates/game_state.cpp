#include "GameStates/game_state.h"
#include "game.h"

GameState::GameState(Game& game, sdl::Renderer& renderer)
	: game_(game), ui_manager_(game.audio_manager_, renderer)
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

Ui* GameState::add_ui_element(std::unique_ptr<Ui> ui)
{
	Ui* ui_ptr = ui.get();
	ui_elements_.push_back(std::move(ui));
	ui_manager_.register_element(ui_ptr);
	return ui_ptr;
}