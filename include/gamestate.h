#pragma once

#include <SDL2/SDL.h>

class GameState
{
	public:
		virtual ~GameState();

		virtual void handle_events(const SDL_Event& e) = 0;
		virtual void draw(SDL_Renderer* renderer) = 0;
		virtual void update(Uint64 time_step) = 0;

	protected:
		GameState();

	private:
};

