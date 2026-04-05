#pragma once

#include "game_state.h"
#include "image.h"

class SettingsMenu : public GameState
{
	public:
		SettingsMenu(Game& game, sdl::Renderer& renderer);

		void create_display_choices(sdl::Renderer& renderer);
		void create_resolutions_scroll_area(sdl::Renderer& renderer);
		void build_ui_elements(sdl::Renderer& renderer);
		void draw(sdl::Renderer& renderer) override;
		void update() override;

		void previous_menu_function(Ui* ui);
		void slider_sound_function(Ui* ui);
		void slider_music_function(Ui* ui);
		void slider_text_function(Ui* ui);
		void texttoggle_full_screen_function(Ui* ui);
		void texttoggle_windowed_function(Ui* ui);
		void texttoggle_resolution_function(Ui* ui);

	private:
		Image background_;
};

