#include "RAII_SDL2/game_controller.h"

namespace sdl
{

GameController::GameController() //SDL_GameControllerOpen
{
	if((game_controller_ = SDL_GameControllerOpen(0)) == nullptr)
	{
		SDL_Log("(SDL_GameControllerOpen) %s\n", SDL_GetError());
	}
}

GameController::GameController(GameController&& game_controller)
	: game_controller_(game_controller.game_controller_)
{
	game_controller.game_controller_ = nullptr;
}

GameController& GameController::operator=(GameController&& game_controller)
{
	if(this == &game_controller)
	{
		return *this;
	}

	if(game_controller_ != nullptr)
	{
		SDL_GameControllerClose(game_controller_);
	}

	game_controller_ = game_controller.game_controller_;
	game_controller.game_controller_ = nullptr;
	return *this;
}

GameController::~GameController() //SDL_GameControllerClose
{
	if(game_controller_ != nullptr)
	{
		SDL_GameControllerClose(game_controller_);
	}
}

SDL_GameController* GameController::fetch() const
{
	return game_controller_;
}

}