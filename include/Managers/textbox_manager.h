#pragma once

#include "GUI/text_button.h"
#include "GUI/text_toggle.h"
#include "textbox.h"
#include "game.h"

class TextboxManager
{
	public:
		enum class Instruction
		{
			NEXT,
			PREV,
			NONE
		};

		struct DialogueInstruction
		{
			Instruction instruction_;
			bool is_input_source_mouse_wheel_;
		};

		struct UiOnTextbox
		{
			TextButton* history_button_;
			TextToggle* skip_toggle_;
			TextToggle* auto_toggle_;
			TextButton* save_button_;
			TextButton* load_button_;
			TextButton* settings_button_;
		};

		TextboxManager(UiManager& ui_manager, UiOnTextbox ui_on_textbox, sdl::Renderer& renderer, Game& game);

		void go_to_next_dialogue(bool is_input_source_mouse_wheel = false);
		void go_to_prev_dialogue(bool is_input_source_mouse_wheel = false);
		void reset_dialogue_instruction(bool is_input_source_mouse_wheel = false);
		bool is_input_source_mouse_wheel() const;
		bool is_dialogue_instruction_next() const;
		bool is_dialogue_instruction_prev() const;
		bool is_dialogue_finished() const;

		std::string get_dialogue();
		std::string get_speaker_name();

		void uncheck_skip_toggle();
		void uncheck_auto_toggle();

		void build_ui_elements(sdl::Renderer& renderer);
		void set_position_ui_textbox(std::string_view position);

		void handle_events_mouse_wheel(const SDL_Event& e);
		void handle_events_keyboard_mouse(const SDL_Event& e);
		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);

		void history_function(Ui* ui);
		void auto_function(Ui* ui);
		void skip_function(Ui* ui);
		void save_function(Ui* ui);
		void load_function(Ui* ui);
		void settings_function(Ui* ui);

		void update_history(const Script::InfoTextbox& info_textbox, const Character& character);
		void update_skip_auto_modes();
		void update_textbox();
		void update(const Script::InfoTextbox& info_textbox, const Character& character);

		void reset(bool is_load);

		bool skip_mode_;

	private:
		DialogueInstruction dialogue_instruction_;
		Uint64 last_time_;
		bool auto_mode_;
		std::string position_;
		Textbox textbox_; 
		UiOnTextbox ui_on_textbox_;

		UiManager& ui_manager_;
		Game& game_;
		sdl::Renderer& renderer_;
};