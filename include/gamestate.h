#pragma once

#include "RAII_SDL2/renderer.h"
#include "GUI/uimanager.h"
#include "audiomanager.h"

#include <SDL2/SDL.h>
#include <vector>
#include <memory>

class Game;

class GameState
{
	public:
		virtual ~GameState() = default;
		virtual void build_ui_elements(sdl::Renderer& renderer) = 0;
		virtual void handle_events(const SDL_Event& e);
		virtual void draw(sdl::Renderer& renderer);
		virtual void update();

	protected:
		GameState(Game& game);
		Game& game_;
		AudioManager& audio_manager_;
		UiManager ui_manager_;
};

