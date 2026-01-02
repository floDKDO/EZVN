#pragma once

#include "gamestate.h"
#include "textbox.h"
#include "character.h"
#include "background.h"
#include "GUI/texttoggle.h"
#include "music.h"
#include "sound.h"
#include <vector>
#include <memory>

class InGame : public GameState
{
	public:
		InGame(Game& game, sdl::Renderer& renderer);

		void build_ui_elements(sdl::Renderer& renderer) override;
		void handle_events(const SDL_Event& e) override;
		void draw(sdl::Renderer& renderer) override;
		void update() override;
		void update2();

		void set_initial_dialogue();

		TextToggle* get_texttoggle(const std::string_view text);

		void add_character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, sdl::Renderer& renderer, const SDL_Color namebox_font_color = constants::namebox_text_color_, const std::string_view textbox_path = "", const std::string_view namebox_path = "");
		std::optional<std::string> get_last_character_name(const std::string_view character_variable);
		CharacterDefinition* get_character_definition(const std::string_view character_variable);

		void insert_dialogue(const std::string_view character_variable, const std::string_view dialogue);

		void insert_background(const std::string_view background_path);
		void insert_background(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a);

		Character* is_character_active(const std::string_view character_variable);
		void move_to_front(const std::string character_variable);
		void show_character(const std::string_view character_variable, const std::optional<std::string> transform_name = std::nullopt, const std::optional<int> zorder = std::nullopt);
		void show_character(const std::string_view character_variable, const std::optional<int> zorder = std::nullopt);
		void hide_character(const std::string_view character_variable);
		void rename_character(const std::string_view character_variable, const std::string_view new_character_name);

		void change_background(const Background& b); //TODO : prendre un InfoBackground ??

		void insert_sound(const std::string_view sound_path, int fadein_length, int fadeout_length, int volume, int channel, bool loop);
		void insert_music(const std::string_view music_path, int fadein_length, int fadeout_length, int volume, bool loop);

		void insert_autofocus(bool autofocus);

		void show_next_dialogue();

		enum class WhichDialogue
		{
			next,
			previous
		};
		void show_dialogue_mouse_wheel(WhichDialogue which_dialogue);

		void auto_function(Ui* ui);
		void skip_function(Ui* ui);
		void settings_function(Ui* ui);
		void temp_function(Ui* ui);

		void draw_characters(sdl::Renderer& renderer);

		void update_backgrounds();
		void update_characters();
		void update_autofocus();
		void update_skip_auto_modes();
		void update_music();
		void update_sounds();
		void update_textbox();

		template <typename T>
		struct MyPair
		{
			std::string character_variable_;
			T t_;
		};

		std::vector<CharacterDefinition> character_definitions_; 
		std::unordered_map<std::string, Character> active_characters_;
		std::vector<std::string> draw_characters_order_;

		struct AudioProperties
		{
			int fadein_length;
			int fadeout_length;
			int volume;
			bool loop;
			int channel; //not used for musics
		};

		using InfoRenameCharacter = MyPair<std::string>;
		using InfoCharacter = MyPair<Character::Editableproperties>;
		using InfoDialogue = MyPair<const std::string_view>;
		using InfoBackground = Background;
		using InfoMusic = std::pair<AudioProperties, std::optional<Music>>;
		using InfoSound = std::pair<AudioProperties, std::optional<Sound>>;
		using InfoAutofocus = bool;
		using ScriptInformation = std::variant<InfoCharacter, InfoRenameCharacter, InfoDialogue, InfoBackground, InfoMusic, InfoSound, InfoAutofocus>;
		std::vector<ScriptInformation> script_information_;

		InGame::InfoDialogue* get_current_dialogue(); 

		//bool one_time_; //avant d'exécuter le script
		std::vector<unsigned int> dialogues_indices_;
		unsigned int current_dialogue_index_; //TODO : utiliser size_t

		//TODO : éventuellement renommer
		bool try_next_dialogue();
		bool try_prev_dialogue();
		void set_next_dialogue_index();
		void set_prev_dialogue_index();
		unsigned int get_prev_dialogue_pos();
		unsigned int get_current_dialogue_pos();
		unsigned int get_next_dialogue_pos();

		//TODO : renommer en current_script_index_
		unsigned int current_unique_id_;

		//TODO : renommer les deux
		struct SavedCurrentUniqueIDWhenPrevious
		{
			bool is_saved_;
			unsigned int saved_value_;
		};
		SavedCurrentUniqueIDWhenPrevious saved_when_prev_;

		Uint64 last_time_;

		Textbox textbox_;

	private:
		bool skip_mode_;
		bool auto_mode_;

		Music* currently_playing_music_;

		struct CurrentSound
		{
			unsigned int associated_dialogue_pos_;
			bool played_;
			Sound* sound_;
		};
		CurrentSound currently_playing_sound_;

		Background background_;
		bool hide_ui_textbox_;
		sdl::Renderer& renderer_;
};