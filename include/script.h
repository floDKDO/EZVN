#pragma once

#include "music.h"
#include "sound.h"
#include "character.h"
#include "background.h"

#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <unordered_map>

class Script
{
	public:
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
			COMPOSITE_IMAGE,
			ZORDER,
			NAME,
			TRANSFORM_NAME,
			IS_VISIBLE,
			TEXTBOX_PATH,
			NAMEBOX_PATH,
			NAMEBOX_TEXT_COLOR
		};

		using CharacterCommandValue = std::variant<unsigned int, std::string, bool, Color>; //composite_image (std::string), zorder (unsigned int), name (std::string), transform_name (std::string), is_visible_ (bool), textbox_path_ (std::string), namebox_path_ (std::string), namebox_text_color_ (Color)
		using CharacterCommands = std::unordered_map<CharacterCommandKind, CharacterCommandValue>;
		using InfoCharacter = MyPair<CharacterCommands>;

		enum class TextboxCommandKind
		{
			DIALOGUE,
			MOVE_TEXTBOX
		};

		struct TextboxCommands
		{
			TextboxCommandKind textbox_command_kind_;
			std::string textbox_command_value_;
		};
		using InfoTextbox = MyPair<TextboxCommands>;

		using InfoBackground = Background;
		using InfoMusic = std::pair<AudioProperties, std::optional<Music>>;
		using InfoSound = std::pair<AudioProperties, std::optional<Sound>>;

		enum class AutofocusCommandKind
		{
			AUTOFOCUS,
			AUTOZORDER
		};

		struct AutofocusCommands
		{
			AutofocusCommandKind autofocus_command_kind_;
			bool autofocus_command_value_;
		};
		using InfoAutofocus = AutofocusCommands;

		using ScriptInformation = std::variant<InfoCharacter, InfoTextbox, InfoBackground, InfoMusic, InfoSound, InfoAutofocus>;

		Script(sdl::Renderer& renderer);

		void insert_dialogue(std::string_view character_variable, std::string_view dialogue);

		/*struct CharacterInfo
		{
			std::string character_variable_;
			std::string composite_image_name_;
		};*/

		void show_character(std::string_view character_variable, std::optional<std::string_view> composite_image_name = std::nullopt, std::optional<std::string_view> transform_name = std::nullopt, std::optional<unsigned int> zorder = std::nullopt);
		void hide_character(std::string_view character_variable);
		void rename_character(std::string_view character_variable, std::string_view new_character_name);

		void add_character(std::string_view character_variable, std::string_view character_name, Color namebox_text_color = constants::namebox_text_color_, std::string_view textbox_path = "", std::string_view namebox_path = "");
		void add_composite_image(std::string_view character_variable, std::string_view composite_image_name, int w, int h, std::initializer_list<ImageInfo> images_info);

		void insert_textbox(std::string_view character_variable, std::string_view textbox_path);
		void insert_namebox(std::string_view character_variable, std::string_view namebox_path);
		void insert_namebox_text_color(std::string_view character_variable, Color namebox_text_color);
		void move_textbox(std::string_view where);

		void insert_background(std::string_view background_path);
		void insert_background(Color color);

		void play_sound(std::string_view sound_path, int fadein_length, int fadeout_length, float volume_multiplier, int channel, bool loop);
		void stop_sound(int fadeout_length, int channel);

		void play_music(std::string_view music_path, int fadein_length, int fadeout_length, float volume_multiplier, bool loop);
		void stop_music(int fadeout_length);

		void insert_autofocus(bool autofocus);
		void insert_autozorder(bool autozorder);

		std::vector<ScriptInformation> script_information_;
		std::unordered_map<std::string, CharacterDefinition> character_definitions_;


	private:
		sdl::Renderer& renderer_;
};

