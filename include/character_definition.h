#pragma once

#include "constants.h"
#include "composite_image.h"

#include <string>
#include <string_view>
#include <unordered_map>

class CharacterDefinition
{
	public:
		CharacterDefinition(std::string_view character_variable, std::string_view character_name, Color namebox_text_color = constants::namebox_text_color_, std::string_view textbox_path = constants::textbox_image_, std::string_view namebox_path = constants::namebox_image_);
		CharacterDefinition(const CharacterDefinition& character_definition) = delete;
		CharacterDefinition(CharacterDefinition&& character_definition) = default;
		CharacterDefinition& operator=(const CharacterDefinition& character_definition) = delete;
		CharacterDefinition& operator=(CharacterDefinition&& character_definition) = default;

		std::string character_variable_;
		std::string initial_name_;

		std::unordered_map<std::string, CompositeImage> composite_images_;

		Color initial_namebox_text_color_;

		std::string initial_textbox_path_;
		std::string initial_namebox_path_;


	private:

};

