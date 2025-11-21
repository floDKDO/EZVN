#include "RAII_SDL2/gamecontroller.h"

namespace sdl
{

GameController::GameController() //SDL_GameControllerOpen
{
	game_controller_ = SDL_GameControllerOpen(0);
}

GameController::~GameController() //SDL_GameControllerClose
{
	SDL_GameControllerClose(game_controller_);
}

SDL_GameController* GameController::Get() const
{
	return game_controller_;
}

}