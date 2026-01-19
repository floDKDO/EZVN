#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <optional>

#include "script.h"

class CharacterManager
{
	public:
		CharacterManager(sdl::Renderer& renderer, std::unordered_map<std::string, CharacterDefinition>& character_definitions);

		void create_narrator();
		void sort_characters_zorder();
		void draw(sdl::Renderer& renderer);
		void update(const Script::InfoCharacter& info_character);
		void reset();

		std::unordered_map<std::string, Character> active_characters_;

	private:
		std::unordered_map<std::string, CharacterDefinition>& character_definitions_;
		std::vector<std::string> draw_characters_order_;
		sdl::Renderer& renderer_;
};

