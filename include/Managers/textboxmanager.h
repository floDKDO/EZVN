#pragma once

#include "GUI/textbutton.h"
#include "GUI/texttoggle.h"
#include "textbox.h"
#include "game.h"

class TextboxManager
{
	public:
		enum class Where
		{
			next,
			prev,
			none
		};

		struct WhichDialogueFromWhere
		{
			Where which_dialogue_;
			bool is_from_mouse_wheel_;
			bool wait_for_end_of_dialogue_;
		};

		TextboxManager(sdl::Renderer& renderer, Game& game);

		void build_ui_elements(sdl::Renderer& renderer);
		void set_position_ui_textbox(std::string_view where);

		void handle_events_mouse_wheel(const SDL_Event& e);
		void handle_events_keyboard_mouse(const SDL_Event& e);
		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);

		void auto_function(Ui* ui);
		void skip_function(Ui* ui);
		void settings_function(Ui* ui);
		void temp_function(Ui* ui);

		void update_skip_auto_modes();
		void update(Script::InfoTextbox& info_textbox, const Character& character);

		void reset();

		WhichDialogueFromWhere which_dialogue_from_where_;

	private:
		Uint64 last_time_;

		bool skip_mode_;
		bool auto_mode_;
		bool hide_ui_textbox_;

		std::string where_;

		Textbox textbox_;
		UiManager ui_manager_;

		TextButton* history_button_;
		TextToggle* skip_toggle_;
		TextToggle* auto_toggle_;
		TextButton* save_button_;
		TextButton* load_button_;
		TextButton* settings_button_;

		Game& game_;
		sdl::Renderer& renderer_;
};