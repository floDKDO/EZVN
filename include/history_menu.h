#pragma once

#include "game_state.h"
#include "image.h"
#include "GUI/scrollable_area.h"

class HistoryMenu : public GameState
{
	public:
		HistoryMenu(Game& game, sdl::Renderer& renderer);

		void create_history_scroll_area(sdl::Renderer& renderer);
		void build_ui_elements(sdl::Renderer& renderer);
		void draw(sdl::Renderer& renderer) override;
		void update() override;

		void add_dialogue_to_history(std::string_view character_name, std::string_view dialogue, Color namebox_text_color, sdl::Renderer& renderer);

		void previous_menu_function(Ui* ui);

		std::unique_ptr<ScrollableArea> scroll_;
		ScrollableArea* scroll_ptr_;
		std::vector<std::pair<Text, Text>> texts_;

	private:
		Image background_;
};
