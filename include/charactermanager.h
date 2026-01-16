#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <optional>

#include "script.h"

class CharacterManager
{
	public:
		CharacterManager(sdl::Renderer& renderer);

		//std::optional<Character::Editableproperties> show_character_prologue(const std::string_view character_variable, sdl::Renderer& renderer);
		void add_character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, Color namebox_text_color = constants::namebox_text_color_, const std::string_view textbox_path = "", const std::string_view namebox_path = "");
		void create_narrator();
		void sort_characters_zorder();

		void draw(sdl::Renderer& renderer);
		void update(const Script::InfoCharacter& info_character);

		std::unordered_map<std::string, CharacterDefinition> character_definitions_;
		std::unordered_map<std::string, Character> active_characters_;
		std::vector<std::string> draw_characters_order_;

	private:
		sdl::Renderer& renderer_;
};

