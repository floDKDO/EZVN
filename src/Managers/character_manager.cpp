#include "Managers/character_manager.h"

#include <algorithm>
#include <iostream>

CharacterManager::CharacterManager(sdl::Renderer& renderer, std::unordered_map<std::string, CharacterDefinition>& character_definitions)
	: autofocus_(constants::default_autofocus_), autozorder_(constants::default_autozorder_), character_definitions_(character_definitions), renderer_(renderer)
{
	create_narrator();
}

void CharacterManager::create_narrator()
{
	CharacterDefinition& character_definition = character_definitions_.at("Narrator");
	active_characters_.insert(std::make_pair("Narrator", Character(character_definition, renderer_)));
}

unsigned int CharacterManager::get_number_of_characters_with_image()
{
	unsigned int counter = 0;
	for(auto& [key_character_variable, value_character] : active_characters_)
	{
		if(key_character_variable != "Narrator" && value_character.has_image())
		{
			counter += 1;
		}
	}
	return counter;
}

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
	sort_characters_zorder(); //TODO : garder ici ou uniquement dans le cas ZORDER ??

	for(const std::string& s : draw_characters_order_)
	{
		active_characters_.at(s).draw(renderer); 
	}
}

void CharacterManager::update(const Script::InfoCharacter& info_character)
{
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
				character.properties_.zorder_ = std::get<unsigned int>(command_value);
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
					character.set_transform("hide");
				}

				//TODO : ce code
				/*if(!character.properties_.is_visible_)
				{
					active_characters_.erase(character_variable);
					for(std::vector<std::string>::iterator it = draw_characters_order_.begin(); it != draw_characters_order_.end(); )
					{
						if(*it == character_variable)
						{
							it = draw_characters_order_.erase(it);
						}
						else
						{
							it += 1;
						}
					}
				}*/
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

void CharacterManager::update_characters()
{
	for(auto& [key_character_variable, value_character] : active_characters_)
	{
		value_character.update();
	}
}

void CharacterManager::update_characters_dialogue(const Script::InfoTextbox& info_textbox)
{
	if(get_number_of_characters_with_image() < constants::default_minimum_character_for_autofocus_) 
	{
		return;
	}

	for(auto& [key_character_variable, value_character] : active_characters_)
	{
		if(autofocus_)
		{
			value_character.properties_.is_speaking_ = false;
		}
		if(autozorder_)
		{
			value_character.properties_.zorder_ = 5;
		}
	}

	//std::cout << "PERSO: " << info_textbox.character_variable_ << ", texte: " << info_textbox.t_.textbox_command_value_ << std::endl;

	Character& character = active_characters_.at(info_textbox.character_variable_);
	if(autofocus_)
	{
		character.properties_.is_speaking_ = true; 
	}
	if(autozorder_)
	{
		character.properties_.zorder_ = 10;
	}
}

void CharacterManager::update_autofocus(const Script::InfoAutofocus& info_autofocus)
{
	if(info_autofocus.autofocus_command_kind_ == Script::AutofocusCommandKind::AUTOFOCUS)
	{
		autofocus_ = info_autofocus.autofocus_command_value_;
	}
	else if(info_autofocus.autofocus_command_kind_ == Script::AutofocusCommandKind::AUTOZORDER)
	{
		autozorder_ = info_autofocus.autofocus_command_value_;
	}
}

void CharacterManager::reset()
{
	autofocus_ = constants::default_autofocus_;
	autozorder_ = constants::default_autozorder_;
	active_characters_.clear();
	draw_characters_order_.clear();
	create_narrator();
	for(auto& [key_character_variable, value_character] : active_characters_)
	{
		value_character.properties_.is_speaking_ = false;
		value_character.properties_.zorder_ = 5;
	}
}