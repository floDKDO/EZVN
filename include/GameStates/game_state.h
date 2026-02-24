#pragma once

#include "RAII_SDL2/renderer.h"
#include "Managers/ui_manager.h"

#include <SDL2/SDL.h>

class Game;

class GameState
{
	public:
		virtual ~GameState() = default;
		virtual void handle_events(const SDL_Event& e);
		virtual void draw(sdl::Renderer& renderer);
		virtual void update();
		virtual void on_show() {};
		virtual void on_hide() {};

	protected:
		explicit GameState(Game& game);
		Game& game_;
		UiManager ui_manager_;
};

