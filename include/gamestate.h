#pragma once

#include <SDL2/SDL.h>

#include "RAII_SDL2/renderer.h"

class GameState
{
	public:
	virtual ~GameState() = default;

		virtual void handle_events(const SDL_Event& e) = 0;
		virtual void draw(sdl::Renderer& renderer) = 0;
		virtual void update(Uint64 time_step) = 0;

	protected:
		GameState() = default;

	private:
};

