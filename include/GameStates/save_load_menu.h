#pragma once

#include "game_state.h"
#include "image.h"
#include "GUI/button.h"

class SaveLoadMenu : public GameState
{
	public:
		SaveLoadMenu(Game& game, sdl::Renderer& renderer);

		void build_ui_elements(sdl::Renderer& renderer);
		void draw(sdl::Renderer& renderer) override;
		void update() override;

		void previous_menu_function(Ui* ui);
		void save_function(Ui* ui);

	private:
		Image background_;
		Button* button_ptr_;

		sdl::Renderer& renderer_;
};