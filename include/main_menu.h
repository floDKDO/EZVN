#pragma once

#include "game_state.h"
#include "image.h"
#include "GUI/scrollbar.h"

class MainMenu : public GameState
{
	public:
		MainMenu(Game& game, std::string_view background_path, sdl::Renderer& renderer);

		void build_ui_elements(sdl::Renderer& renderer);
		void draw(sdl::Renderer& renderer) override;
		void update() override;

		void temp_function(Ui* ui);
		void play_function(Ui* ui);
		void settings_function(Ui* ui);
		void confirmationpopup_quit_function(Ui* ui);

		Scrollbar* scrollbar_;

	private:
		Image background_;
};

