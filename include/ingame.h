#pragma once

#include "gamestate.h"
#include "textbox.h"
#include "character.h"
#include "background.h"
#include "GUI/textbutton.h"
#include "GUI/texttoggle.h"
#include "music.h"
#include "RAII_SDL2/chunk.h"
#include "script.h"

#include <vector>
#include <unordered_set>

class InGame : public GameState
{
	enum class WhichDialogue
	{
		none,
		next,
		prev 
	};

	struct WhichDialogueFromWhere
	{
		WhichDialogue which_dialogue_;
		bool is_from_mouse_wheel_;
		bool wait_for_end_of_dialogue_;
	};

	struct CurrentSound
	{
		struct Script::AudioProperties audio_properties_;
		size_t associated_script_index_;
		bool played_;
		Sound* sound_;
	};

	struct CurrentMusic
	{
		struct Script::AudioProperties audio_properties_;
		Music* music_;
	};


	public:
		InGame(Game& game, sdl::Renderer& renderer);

		void build_ui_elements(sdl::Renderer& renderer) override;
		void handle_events(const SDL_Event& e) override;
		void draw(sdl::Renderer& renderer) override;
		void update() override;
		void update2();

		void auto_function(Ui* ui);
		void skip_function(Ui* ui);
		void settings_function(Ui* ui);
		void temp_function(Ui* ui);

		bool move_dialogue();

		std::optional<Character::Editableproperties> show_character_prologue(const std::string_view character_variable);
		void add_character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, Color namebox_text_color = constants::namebox_text_color_, const std::string_view textbox_path = "", const std::string_view namebox_path = "");
		void create_narrator();
		Character* is_character_active(const std::string_view character_variable);
		void draw_characters(sdl::Renderer& renderer);

		void change_background(const Script::InfoBackground& b); 

		void update_backgrounds(const Script::InfoBackground& info_background);
		void update_characters(const Script::InfoCharacter& info_character);
		void update_characters_dialogue(Script::InfoDialogue& info_dialogue);
		void update_autofocus(const Script::InfoAutofocus& info_autofocus);
		void update_skip_auto_modes();
		void update_music(Script::InfoMusic& info_music);

		void halt_all_sounds();
		void update_sounds(Script::InfoSound& info_sound, size_t i);

		void update_textbox(Script::InfoTextbox& info_textbox);
		void update_dialogue(Script::InfoDialogue& info_dialogue);

	private:
		std::unordered_map<std::string, CharacterDefinition> character_definitions_;
		std::unordered_map<std::string, Character> active_characters_;
		std::vector<std::string> draw_characters_order_;

		WhichDialogueFromWhere which_dialogue_from_where_;

		bool skip_mode_;
		bool auto_mode_;
		bool autofocus_;

		Uint64 last_time_;

		Background background_;
		Textbox textbox_;

		TextButton* history_button_;
		TextToggle* skip_toggle_;
		TextToggle* auto_toggle_;
		TextButton* save_button_;
		TextButton* load_button_;
		TextButton* settings_button_;

		bool hide_ui_textbox_;

		CurrentMusic currently_playing_music_;  //TODO renommer pour coller avec le user-defined event de fin de musique ??
		CurrentSound currently_playing_sound_;

		sdl::Renderer& renderer_;
};