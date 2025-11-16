#include "RAII_SDL2/gamecontroller.h"

namespace sdl
{

GameController::GameController() //SDL_GameControllerOpen
{
	this->game_controller = SDL_GameControllerOpen(0);
}

GameController::~GameController() //SDL_GameControllerClose
{
	SDL_GameControllerClose(this->game_controller);
}

SDL_GameController* GameController::Get() const
{
	return this->game_controller;
}

}