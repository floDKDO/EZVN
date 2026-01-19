#pragma once

#include "gamestate.h"
#include "GUI/textbutton.h"
#include "GUI/texttoggle.h"
#include "scriptrunner.h"

class InGame : public GameState
{
	public:
		InGame(Game& game, sdl::Renderer& renderer);

		void build_ui_elements(sdl::Renderer& renderer) override;
		void handle_events(const SDL_Event& e) override;
		void draw(sdl::Renderer& renderer) override;
		void update() override;

		void set_position_ui_textbox(std::string_view where);

		void auto_function(Ui* ui);
		void skip_function(Ui* ui);
		void settings_function(Ui* ui);
		void temp_function(Ui* ui);

		void update_characters_dialogue(Script::InfoDialogue& info_dialogue);
		void update_skip_auto_modes();

		void update_textbox(Script::InfoTextbox& info_textbox);

		ScriptRunner script_runner_;

	private:
		bool skip_mode_;
		bool auto_mode_;
		bool autofocus_;

		TextButton* history_button_;
		TextToggle* skip_toggle_;
		TextToggle* auto_toggle_;
		TextButton* save_button_;
		TextButton* load_button_;
		TextButton* settings_button_;

		bool hide_ui_textbox_;

		sdl::Renderer& renderer_;
};