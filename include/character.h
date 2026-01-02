#pragma once

#include "character_definition.h"
#include "transform.h"

class Character
{
	public:
		Character(const CharacterDefinition& character_definition, sdl::Renderer& renderer, const std::string transform_name="", const int zorder=0);

		void set_transform(const std::string transform_name);
		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update();

		const CharacterDefinition* character_definition_; //TODO : garder const ??

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

