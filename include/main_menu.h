#pragma once

#include "game_state.h"
#include "image.h"
#include "GUI/scrollbar.h"
#include "GUI/scrollable_area.h"
#include "GUI/text_button.h" //TODO : à retirer

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

		std::unique_ptr<ScrollableArea> scroll_;
		TextButton* button_0_;
		TextButton* button_1_;
		TextButton* button_2_;
		TextButton* button_3_;
		TextButton* button_4_;

		Scrollbar* scrollbar_;

	private:
		Image background_;
};

