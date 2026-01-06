#include "character_definition.h"

CharacterDefinition::CharacterDefinition(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, const SDL_Color namebox_text_color, const std::string_view textbox_path, const std::string_view namebox_path)
	: character_variable_(character_variable), initial_name_(character_name), character_path_(character_path), initial_namebox_text_color_(namebox_text_color), initial_textbox_path_(textbox_path), initial_namebox_path_(namebox_path)
{}