#pragma once

#include "character_definition.h"
#include "transform.h"

class Character
{
	public:
		Character(const CharacterDefinition& character_definition, sdl::Renderer& renderer, const std::string transform_name="", const int zorder=0);
		Character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, sdl::Renderer& renderer);
		Character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, sdl::Renderer& renderer, const SDL_Color namebox_font_color, const std::string_view textbox_path, const std::string_view namebox_path);

		void set_transform(const std::string transform_name);
		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update();

		CharacterDefinition character_definition_;

		struct Editableproperties
		{
			std::string name_; //garder le type std::string pour permettre au joueur de taper son nom au clavier
			int zorder_;
			std::string textbox_path_;
			std::string namebox_path_;
			Transform transform_;
			bool is_speaking_;
			bool is_visible_;
		};
		Editableproperties properties_;
		Image character_; //TODO : juste pour tester => truc similaire à MPT par la suite

	private:
		//SDL_Rect initial_rect_;
};

