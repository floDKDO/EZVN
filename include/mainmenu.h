#pragma once

#include "gamestate.h"
#include "image.h"
#include "music.h"

class MainMenu : public GameState
{
	public:
		MainMenu(Game& game, const std::string_view background_path, sdl::Renderer& renderer);

		void build_ui_elements(sdl::Renderer& renderer) override;
		void draw(sdl::Renderer& renderer) override;

		void temp_function(Ui* ui);
		void play_function(Ui* ui);
		void settings_function(Ui* ui);
		void confirmationpopup_quit_function(Ui* ui);

	private:
		Image background_;
		Music music_;
};

