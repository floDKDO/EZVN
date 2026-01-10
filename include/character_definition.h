#pragma once

#include "constants.h"

#include <string>
#include <SDL2/SDL.h>

class CharacterDefinition
{
	public:
		CharacterDefinition(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, Color namebox_text_color = constants::namebox_text_color_, const std::string_view textbox_path = constants::textbox_image_, const std::string_view namebox_path = constants::namebox_image_);
		CharacterDefinition(const CharacterDefinition& character_definition) = delete;
		CharacterDefinition(CharacterDefinition&& character_definition) = default;
		CharacterDefinition& operator=(const CharacterDefinition& character_definition) = delete;
		CharacterDefinition& operator=(CharacterDefinition&& character_definition) = default;

		std::string character_variable_;
		std::string initial_name_;

		std::string character_path_;

		Color initial_namebox_text_color_;

		std::string initial_textbox_path_;
		std::string initial_namebox_path_;


	private:

};

