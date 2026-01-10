#pragma once

#include "gamestate.h"
#include "textbox.h"
#include "character.h"
#include "background.h"
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

	struct ScriptIndexWhenPrev
	{
		bool is_saved_;
		size_t saved_script_index_;
	};

	struct AudioProperties
	{
		int fadein_length_;
		int fadeout_length_;
		float volume_multiplier_;
		bool loop_;
		int channel_; //not used for musics
	};

	struct CurrentSound
	{
		struct AudioProperties audio_properties_;
		size_t associated_script_index_;
		bool played_;
		sdl::Chunk* sound_;
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
	using InfoSound = std::pair<AudioProperties, std::optional<sdl::Chunk>>;
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
		void insert_textbox(const std::string_view character_variable, const std::string_view textbox_path);
		void insert_namebox(const std::string_view character_variable, const std::string_view namebox_path);
		void insert_namebox_text_color(const std::string_view character_variable, Color namebox_text_color);

		void insert_background(const std::string_view background_path);
		void insert_background(Color color);

		void play_sound(const std::string_view sound_path, int fadein_length, int fadeout_length, float volume_multiplier, int channel, bool loop);
		void stop_sound(int fadeout_length, int channel);

		void play_music(const std::string_view music_path, int fadein_length, int fadeout_length, float volume_multiplier, bool loop);
		void stop_music(int fadeout_length);

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

		void add_character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, Color namebox_text_color = constants::namebox_text_color_, const std::string_view textbox_path = "", const std::string_view namebox_path = "");
		void create_narrator();
		Character* is_character_active(const std::string_view character_variable);
		std::optional<Character::Editableproperties> get_last_character_properties(const std::string_view character_variable);
		void draw_characters(sdl::Renderer& renderer);

		void change_background(const InfoBackground& b); 

		void update_current_script_index_dialogue();
		void update_backgrounds(const InfoBackground& info_background);
		void update_characters(const InfoCharacter& info_character);
		void update_autofocus(const InfoAutofocus& info_autofocus);
		void update_skip_auto_modes();
		void update_music(InfoMusic& info_music);
		static void callback();

		void halt_all_sounds();
		void update_sounds(InfoSound& info_sound, size_t i);

		void update_textbox();
		void update_dialogue();

	private:
		std::unordered_map<std::string, CharacterDefinition> character_definitions_;
		std::unordered_map<std::string, Character> active_characters_;
		std::unordered_set<std::string> character_variables_seen_; //for update_characters()
		std::vector<std::string> draw_characters_order_;
		std::vector<size_t> dialogues_indices_;
		std::vector<ScriptInformation> script_information_;

		size_t current_dialogue_index_; 
		size_t current_script_index_;
		size_t previous_script_index_;

		ScriptIndexWhenPrev script_index_when_prev_;
		WhichDialogueFromWhere which_dialogue_from_where_;

		bool skip_mode_;
		bool auto_mode_;

		Uint64 last_time_;

		Background background_;
		Textbox textbox_;
		bool hide_ui_textbox_;

		CurrentMusic currently_playing_music_; 
		CurrentSound currently_playing_sound_;

		bool background_changed_;
		bool autofocus_changed_;
		bool music_changed_;
		bool sound_changed_;

		sdl::Renderer& renderer_;
};