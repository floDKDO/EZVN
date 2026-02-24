#pragma once

#include "game_state.h"
#include "script_runner.h"

class InGame : public GameState
{
	public:
		InGame(Game& game, sdl::Renderer& renderer);

		void handle_events(const SDL_Event& e) override;
		void draw(sdl::Renderer& renderer) override;
		void update() override;

		ScriptRunner script_runner_;

	private:
		sdl::Renderer& renderer_;
};