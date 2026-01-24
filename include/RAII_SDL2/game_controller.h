#pragma once

#include <SDL2/SDL.h>

namespace sdl
{

class GameController
{
	public:
		GameController(); //SDL_GameControllerOpen
		GameController(const GameController& game_controller) = delete;
		GameController(GameController&& game_controller);
		GameController& operator=(const GameController& game_controller) = delete;
		GameController& operator=(GameController&& game_controller);
		~GameController(); //SDL_GameControllerClose

		SDL_GameController* fetch() const;

	private:
		SDL_GameController* game_controller_;
};
}
