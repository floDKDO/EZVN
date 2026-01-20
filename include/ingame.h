#pragma once

#include "gamestate.h"
#include "scriptrunner.h"

class InGame : public GameState
{
	public:
		InGame(Game& game, sdl::Renderer& renderer);

		void handle_events(const SDL_Event& e) override;
		void draw(sdl::Renderer& renderer) override;
		void update() override;

		void update_characters_dialogue(Script::InfoTextbox& info_dialogue);
		void update_skip_auto_modes();

		ScriptRunner script_runner_;

	private:
		sdl::Renderer& renderer_;
};