#pragma once

#include "gamestate.h"
#include "textbox.h"
#include "character.h"
#include "GUI/uimanager.h"
#include <vector>
#include <memory>

class InGameState : public GameState
{
	public:
		explicit InGameState(sdl::Renderer& renderer);
		void handle_events(const SDL_Event& e) override;
		void draw(sdl::Renderer& renderer) override;
		void update() override;

		void temp_function(Ui* ui);

	protected:

	private:
		std::unique_ptr<UiManager> menu_;
		Textbox textbox_;
		std::vector<std::unique_ptr<Character>> characters_;
		Image background_;

		bool hide_ui_textbox_;

		sdl::Renderer& renderer_;
};

