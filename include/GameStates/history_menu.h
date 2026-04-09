#pragma once

#include "game_state.h"
#include "image.h"
#include "GUI/scrollable_area.h"

class HistoryMenu : public GameState
{
	public:
		HistoryMenu(Game& game, sdl::Renderer& renderer);

		void build_ui_elements(sdl::Renderer& renderer);
		void draw(sdl::Renderer& renderer) override;
		void update() override;
		void on_show() override;

		void clear();
		void add_dialogue_to_history(std::string_view character_name, std::string_view dialogue, Color namebox_text_color, int textbox_width, sdl::Renderer& renderer);
		void remove_last_dialogue_from_history();

		void previous_menu_function(Ui* ui);

		ScrollableArea* scroll_ptr_;
		std::vector<std::pair<Text, Text>> texts_;

	private:
		Image background_;
};
