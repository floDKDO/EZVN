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

		Ui* add_ui_element(std::unique_ptr<Ui> ui);

	protected:
		GameState(Game& game, sdl::Renderer& renderer);
		Game& game_;
		UiManager ui_manager_;

		std::vector<std::unique_ptr<Ui>> ui_elements_;
};

