#pragma once

#include "gamestate.h"
#include "textbox.h"
#include "character.h"
#include "background.h"
#include "GUI/textbutton.h"
#include "GUI/texttoggle.h"
#include "music.h"
#include "RAII_SDL2/chunk.h"

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

	struct AudioProperties
	{
		int fadein_length_;
		int fadeout_length_;
		bool loop_;
		int channel_; //not used for musics
	};

	struct CurrentSound
	{
		struct AudioProperties audio_properties_;
		size_t associated_script_index_;
		bool played_;
		Sound* sound_;
	};

	template <typename T>
	struct MyPair
	{
		std::string character_variable_;
		T t_;
	};

	struct CurrentMusic
	{
		struct AudioProperties audio_properties_;
		Music* music_;
	};

	using InfoCharacter = MyPair<Character::Editableproperties>;
	using InfoDialogue = MyPair<const std::string>;
	using InfoBackground = Background;
	using InfoMusic = std::pair<AudioProperties, std::optional<Music>>;
	using InfoSound = std::pair<AudioProperties, std::optional<Sound>>;
	using InfoAutofocus = bool;
	using InfoTextbox = std::string;
	using ScriptInformation = std::variant<InfoCharacter, InfoDialogue, InfoBackground, InfoMusic, InfoSound, InfoAutofocus, InfoTextbox>;


	struct Script
	{
		size_t current_script_index_;
		size_t previous_script_index_;
		std::vector<ScriptInformation> script_information_;
	};


	public:
		InGame(Game& game, sdl::Renderer& renderer);

		void build_ui_elements(sdl::Renderer& renderer) override;
		void handle_events(const SDL_Event& e) override;
		void draw(sdl::Renderer& renderer) override;
		void update() override;
		void update2();

		TextToggle* get_texttoggle(const std::string_view text);

		void auto_function(Ui* ui);
		void skip_function(Ui* ui);
		void settings_function(Ui* ui);
		void temp_function(Ui* ui);


		void insert_dialogue(const std::string_view character_variable, const std::string_view dialogue);

		Character::Editableproperties show_character_prologue(const std::string_view character_variable);
		void show_character(const std::string_view character_variable, const std::optional<std::string> transform_name = std::nullopt, const std::optional<int> zorder = std::nullopt);
		void hide_character(const std::string_view character_variable);
		void rename_character(const std::string_view character_variable, const std::string_view new_character_name);
		void insert_textbox(const std::string_view character_variable, const std::string_view textbox_path);
		void insert_namebox(const std::string_view character_variable, const std::string_view namebox_path);
		void insert_namebox_text_color(const std::string_view character_variable, Color namebox_text_color);
		void move_textbox(const std::string_view where);

		void insert_background(const std::string_view background_path);
		void insert_background(Color color);

		void play_sound(const std::string_view sound_path, int fadein_length, int fadeout_length, float volume_multiplier, int channel, bool loop);
		void stop_sound(int fadeout_length, int channel);

		void play_music(const std::string_view music_path, int fadein_length, int fadeout_length, float volume_multiplier, bool loop);
		void stop_music(int fadeout_length);

		void insert_autofocus(bool autofocus);

		bool move_dialogue();

		void add_character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, Color namebox_text_color = constants::namebox_text_color_, const std::string_view textbox_path = "", const std::string_view namebox_path = "");
		void create_narrator();
		Character* is_character_active(const std::string_view character_variable);
		std::optional<Character::Editableproperties> get_last_character_properties(const std::string_view character_variable);
		void draw_characters(sdl::Renderer& renderer);

		void change_background(const InfoBackground& b); 

		void update_backgrounds(const InfoBackground& info_background);
		void update_characters(const InfoCharacter& info_character);
		void update_characters_dialogue(InfoDialogue& info_dialogue);
		void update_autofocus(const InfoAutofocus& info_autofocus);
		void update_skip_auto_modes();
		void update_music(InfoMusic& info_music);

		void halt_all_sounds();
		void update_sounds(InfoSound& info_sound, size_t i);

		void update_textbox(InfoTextbox& info_textbox);
		void update_dialogue(InfoDialogue& info_dialogue);

	private:
		std::unordered_map<std::string, CharacterDefinition> character_definitions_;
		std::unordered_map<std::string, Character> active_characters_;
		std::vector<std::string> draw_characters_order_;

		Script script_;

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