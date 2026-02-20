#pragma once

#include "game_state.h"
#include "image.h"
#include "GUI/scrollable_area.h"
#include "GUI/ui_group.h"

class SettingsMenu : public GameState
{
	public:
		SettingsMenu(Game& game, std::string_view background_path, sdl::Renderer& renderer);

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

		std::unique_ptr<ScrollableArea> scroll_;
		std::unique_ptr<UiGroup> ui_group_;

	private:
		Image background_;
};

