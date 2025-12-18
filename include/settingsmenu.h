#pragma once

#include "gamestate.h"
#include "image.h"

class SettingsMenu : public GameState
{
	public:
		SettingsMenu(Game& game, const std::string_view background_path, sdl::Renderer& renderer);

		void build_ui_elements(sdl::Renderer& renderer) override;
		void draw(sdl::Renderer& renderer) override;

		void previous_menu_function(Ui* ui);
		void slider_sound_function(Ui* ui);
		void slider_music_function(Ui* ui);
		void slider_text_function(Ui* ui);
		void texttoggle_full_screen_function(Ui* ui);
		void texttoggle_windowed_function(Ui* ui);

	private:
		Image background_;
};

