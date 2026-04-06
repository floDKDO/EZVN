#pragma once

#include "game_state.h"
#include "script_runner.h"

class InGame : public GameState
{
	public:
		InGame(Game& game, sdl::Renderer& renderer);

		void build_ui_elements(sdl::Renderer& renderer);
		void handle_events(const SDL_Event& e) override;
		void draw(sdl::Renderer& renderer) override;
		void update() override;

		std::unique_ptr<ScriptRunner> script_runner_;
		TextboxManager::UiOnTextbox ui_on_textbox_;

	private:
		sdl::Renderer& renderer_;
};