#pragma once

#include "game.h"
#include "GUI/ui_group.h"

class ChoiceMenuManager
{
	public:
		ChoiceMenuManager(UiManager& ui_manager, sdl::Renderer& renderer, Game& game);

		void clear_before_update();
		void handle_events(const SDL_Event& e);
		void build_ui_elements(sdl::Renderer& renderer);
		void draw(sdl::Renderer& renderer);
		void update(const Script::InfoChoiceMenu& info_choice_menu);
		void update_buttons();
		void reset();

		bool choice_made_;
		bool is_visible_;
		std::vector<Script::ChoiceMenuNextDialogue> all_after_choice_dialogues_;
		Script::ChoiceMenuNextDialogue after_choice_dialogue_;

	private:
		std::unique_ptr<UiGroup> ui_group_;
		UiManager& ui_manager_;
		sdl::Renderer& renderer_;
};

