#pragma once

#include "gamestate.h"
#include "textbox.h"
#include "character.h"
#include "background.h"
#include "GUI/texttoggle.h"
#include "music.h"
#include "sound.h"
#include <vector>

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

	struct ScriptIndexWhenPrev
	{
		bool is_saved_;
		size_t saved_script_index_;
	};

	struct CurrentSound
	{
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

	struct AudioProperties
	{
		int fadein_length_;
		int fadeout_length_;
		int volume_;
		bool loop_;
		int channel_; //not used for musics
	};

	using InfoCharacter = MyPair<Character::Editableproperties>;
	using InfoDialogue = MyPair<const std::string>;
	using InfoBackground = Background;
	using InfoMusic = std::pair<AudioProperties, std::optional<Music>>;
	using InfoSound = std::pair<AudioProperties, std::optional<Sound>>;
	using InfoAutofocus = bool;
	using ScriptInformation = std::variant<InfoCharacter, InfoDialogue, InfoBackground, InfoMusic, InfoSound, InfoAutofocus>;

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

		void insert_background(const std::string_view background_path);
		void insert_background(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a);

		void insert_sound(const std::string_view sound_path, int fadein_length, int fadeout_length, int volume, int channel, bool loop);
		void insert_music(const std::string_view music_path, int fadein_length, int fadeout_length, int volume, bool loop);

		void insert_autofocus(bool autofocus);

		InGame::InfoDialogue* get_current_dialogue();
		std::string get_last_character_name();
		bool does_next_dialogue_exist(); 
		bool does_prev_dialogue_exist();
		size_t get_prev_script_index();
		size_t get_current_script_index();
		size_t get_next_script_index();
		void show_next_dialogue();
		void show_dialogue_mouse_wheel();

		void add_character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, const SDL_Color namebox_font_color = constants::namebox_text_color_, const std::string_view textbox_path = "", const std::string_view namebox_path = "");
		void create_narrator();
		Character* is_character_active(const std::string_view character_variable);
		std::optional<Character::Editableproperties> get_last_character_properties(const std::string_view character_variable);
		void draw_characters(sdl::Renderer& renderer);

		void change_background(const InfoBackground& b); 

		void update_current_script_index_dialogue();
		void update_backgrounds();
		void update_characters();
		void update_autofocus();
		void update_skip_auto_modes();
		void update_music();

		void halt_all_sounds();
		void update_sounds();

		void update_textbox();
		void update_dialogue();

	private:
		std::unordered_map<std::string, CharacterDefinition> character_definitions_;
		std::unordered_map<std::string, Character> active_characters_;
		std::vector<std::string> draw_characters_order_;
		std::vector<size_t> dialogues_indices_;
		std::vector<ScriptInformation> script_information_;

		size_t current_dialogue_index_; 
		size_t current_script_index_;

		ScriptIndexWhenPrev script_index_when_prev_;
		WhichDialogueFromWhere which_dialogue_from_where_;

		bool skip_mode_;
		bool auto_mode_;

		Uint64 last_time_;

		Background background_;
		Textbox textbox_;
		bool hide_ui_textbox_;

		Music* currently_playing_music_;
		CurrentSound currently_playing_sound_;

		sdl::Renderer& renderer_;
};