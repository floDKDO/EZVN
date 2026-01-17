#pragma once

#include "music.h"
#include "sound.h"
#include "character.h"
#include "background.h"

#include <string>
#include <vector>
#include <variant>
#include <optional>

class Script
{
	public:
		Script(sdl::Renderer& renderer);

		struct ScriptIndexWhenPrev
		{
			bool is_saved_;
			size_t saved_script_index_;
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
			bool loop_;
			int channel_; //not used for musics
		};

		enum class CharacterCommandKind
		{
			ZORDER,
			NAME,
			TRANSFORM_NAME,
			IS_VISIBLE,
			TEXTBOX_PATH,
			NAMEBOX_PATH,
			NAMEBOX_TEXT_COLOR
		};

		using CharacterCommandValue = std::variant<int, std::string, bool, Color>; //zorder (int), name (std::string), transform_name (std::string), is_visible_ (bool), textbox_path_ (std::string), namebox_path_ (std::string), namebox_text_color_ (Color)
		using CharacterCommands = std::unordered_map<CharacterCommandKind, CharacterCommandValue>;

		using InfoCharacter = MyPair<CharacterCommands>;
		using InfoDialogue = MyPair<const std::string>;
		using InfoBackground = Background;
		using InfoMusic = std::pair<AudioProperties, std::optional<Music>>;
		using InfoSound = std::pair<AudioProperties, std::optional<Sound>>;
		using InfoAutofocus = bool;
		using InfoTextbox = std::string;
		using ScriptInformation = std::variant<InfoCharacter, InfoDialogue, InfoBackground, InfoMusic, InfoSound, InfoAutofocus, InfoTextbox>;

		void insert_dialogue(const std::string_view character_variable, const std::string_view dialogue);

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

		void increment_script_index();
		void decrement_script_index();
		bool is_current_script_index_a_dialogue();

		enum class Where
		{
			next,
			prev,
			none
		};
		bool move_dialogue(Where where, bool is_from_mouse_wheel_);

		size_t current_script_index_;
		size_t previous_script_index_;
		ScriptIndexWhenPrev script_index_when_prev_;
		std::vector<ScriptInformation> script_information_;

	private:
		sdl::Renderer& renderer_;
};

