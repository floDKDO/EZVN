#pragma once

#include "gamestate.h"
#include "image.h"

class MainMenu : public GameState
{
	public:
		MainMenu(Game& game, const std::string_view background_path, sdl::Renderer& renderer);

		void build_ui_elements(sdl::Renderer& renderer) override;
		void handle_events(const SDL_Event& e) override;
		void draw(sdl::Renderer& renderer) override;
		void update() override;

		void play_function(Ui* ui);
		void settings_function(Ui* ui);
		void confirmationpopup_quit_function(Ui* ui);

	private:
		Image background_;
};

