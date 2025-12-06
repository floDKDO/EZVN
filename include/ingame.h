#pragma once

#include "gamestate.h"
#include "textbox.h"
#include "character.h"
#include <vector>
#include <memory>

class InGame : public GameState
{
	public:
		InGame(Game& game, sdl::Renderer& renderer);

		void build_ui_elements(sdl::Renderer& renderer) override;
		void handle_events(const SDL_Event& e) override;
		void draw(sdl::Renderer& renderer) override;
		void update() override;

		void temp_function(Ui* ui);

	private:
		std::vector<std::unique_ptr<Character>> characters_;

		Textbox textbox_;
		Image background_;
		bool hide_ui_textbox_;
};

