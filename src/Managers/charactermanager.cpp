#include "Managers/charactermanager.h"
#include <algorithm>

//Character//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CharacterManager::CharacterManager(sdl::Renderer& renderer, std::unordered_map<std::string, CharacterDefinition>& character_definitions)
	: character_definitions_(character_definitions), renderer_(renderer)
{
	create_narrator();
}

//Character
void CharacterManager::create_narrator()
{
	CharacterDefinition& character_definition = character_definitions_.at("Narrator");
	active_characters_.insert(std::make_pair("Narrator", Character(character_definition, renderer_)));
}

//Character
void CharacterManager::sort_characters_zorder()
{
	std::stable_sort(draw_characters_order_.begin(), draw_characters_order_.end(),
		[&](const std::string_view a, const std::string_view b) -> bool
	{
		return active_characters_.at(std::string(a)).properties_.zorder_ < active_characters_.at(std::string(b)).properties_.zorder_;
	});
}

void CharacterManager::draw(sdl::Renderer& renderer)
{
	//Characters
	sort_characters_zorder(); //TODO : garder ici ou uniquement dans le cas ZORDER ??

	for(const std::string& s : draw_characters_order_)
	{
		active_characters_.at(s).draw(renderer); 
	}
}

void CharacterManager::update(const Script::InfoCharacter& info_character)
{
	//Characters
	const std::string& character_variable = info_character.character_variable_;

	if(!active_characters_.count(character_variable))
	{
		CharacterDefinition& character_definition = character_definitions_.at(character_variable);
		active_characters_.insert(std::make_pair(character_variable, Character(character_definition, renderer_)));
		draw_characters_order_.push_back(character_variable);
	}

	Character& character = active_characters_.at(character_variable);

	for(auto& [command_kind, command_value] : info_character.t_)
	{
		switch(command_kind)
		{
			case Script::CharacterCommandKind::ZORDER:
				character.properties_.zorder_ = std::get<int>(command_value);
				//sort_characters_zorder();
				break;

			case Script::CharacterCommandKind::NAME:
				character.properties_.name_ = std::get<std::string>(command_value);
				break;

			case Script::CharacterCommandKind::TRANSFORM_NAME:
				character.properties_.transform_name_ = std::get<std::string>(command_value);
				character.set_transform(character.properties_.transform_name_);
				break;

			case Script::CharacterCommandKind::IS_VISIBLE:
				character.properties_.is_visible_ = std::get<bool>(command_value);
				if(!character.properties_.is_visible_)
				{
					active_characters_.erase(character_variable);
					for(std::vector<std::string>::iterator it = draw_characters_order_.begin(); it != draw_characters_order_.end(); ++it)
					{
						if(*it == character_variable)
						{
							draw_characters_order_.erase(it);
						}
					}
				}
				break;

			case Script::CharacterCommandKind::TEXTBOX_PATH:
				character.properties_.textbox_path_ = std::get<std::string>(command_value);
				break;

			case Script::CharacterCommandKind::NAMEBOX_PATH:
				character.properties_.namebox_path_ = std::get<std::string>(command_value);
				break;

			case Script::CharacterCommandKind::NAMEBOX_TEXT_COLOR:
				character.properties_.namebox_text_color_ = std::get<Color>(command_value);
				break;

			default:
				std::cerr << "Unknown character command...\n";
				break;
		}
	}
}

void CharacterManager::reset()
{
	active_characters_.clear();
	draw_characters_order_.clear();
	create_narrator();
}