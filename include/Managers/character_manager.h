#pragma once

#include "script.h"

#include <vector>
#include <string>
#include <unordered_map>

class CharacterManager
{
	public:
		CharacterManager(sdl::Renderer& renderer, std::unordered_map<std::string, CharacterDefinition>& character_definitions);

		void create_narrator();
		unsigned int get_number_of_characters_with_image();
		void sort_characters_zorder();
		void draw(sdl::Renderer& renderer);
		void update(const Script::InfoCharacter& info_character);
		void update_characters();
		void update_characters_dialogue(const Script::InfoTextbox& info_textbox);
		void update_autofocus(const Script::InfoAutofocus& info_autofocus);
		void reset();

		std::unordered_map<std::string, Character> active_characters_;
		bool autofocus_;
		bool autozorder_;

	private:
		std::unordered_map<std::string, CharacterDefinition>& character_definitions_;
		std::vector<std::string> draw_characters_order_;
		sdl::Renderer& renderer_;
};

