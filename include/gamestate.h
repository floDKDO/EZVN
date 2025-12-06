#pragma once

#include "RAII_SDL2/renderer.h"
#include "GUI/ui.h"

#include <SDL2/SDL.h>
#include <vector>
#include <memory>

class Game;

class GameState
{
	public:
		virtual ~GameState() = default;
		virtual void handle_events(const SDL_Event& e) = 0;
		virtual void draw(sdl::Renderer& renderer) = 0;
		virtual void update() = 0;

		std::vector<std::unique_ptr<Ui>> ui_elements_;

	protected:
		GameState(Game& game);
		Game& game_;

		virtual void build_ui_elements(sdl::Renderer& renderer) = 0;
};

