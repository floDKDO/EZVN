#pragma once

#include "game.h"
#include "GUI/ui_group.h"

class ChoiceMenuManager
{
	public:
		ChoiceMenuManager(sdl::Renderer& renderer, Game& game);

		void handle_events(const SDL_Event& e);
		void build_ui_elements(sdl::Renderer& renderer);
		void draw(sdl::Renderer& renderer);
		void update(const Script::InfoChoiceMenu& info_choice_menu);
		void reset();

		bool choice_made_;

	private:
		UiGroup* ui_group_;
		UiManager ui_manager_;
		sdl::Renderer& renderer_;
};

