#include "character_definition.h"

CharacterDefinition::CharacterDefinition(std::string_view character_variable, std::string_view character_name, std::string_view character_path, Color namebox_text_color, std::string_view textbox_path, std::string_view namebox_path)
	: character_variable_(character_variable), initial_name_(character_name), character_path_(character_path), initial_namebox_text_color_(namebox_text_color), initial_textbox_path_(textbox_path), initial_namebox_path_(namebox_path)
{}