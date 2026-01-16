#pragma once

#include "character_definition.h"
#include "transform.h"

class Character
{
	public:
		Character(const CharacterDefinition& character_definition, sdl::Renderer& renderer);

		void set_transform(const std::string transform_name);
		//void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update();

		const CharacterDefinition* character_definition_; //TODO : garder const ??

		struct Editableproperties
		{
			Editableproperties()
				: name_(""), zorder_(constants::default_zorder_), namebox_text_color_(constants::namebox_text_color_), textbox_path_(""), namebox_path_(""), transform_name_("none"), is_speaking_(false), is_visible_(false)
			{}

			Editableproperties(std::string name, int zorder, Color namebox_text_color, std::string textbox_path, std::string namebox_path, std::string transform_name, bool is_speaking, bool is_visible)
				: name_(name), zorder_(zorder), namebox_text_color_(namebox_text_color), textbox_path_(textbox_path), namebox_path_(namebox_path), transform_name_(transform_name), is_speaking_(is_speaking), is_visible_(is_visible)
			{}

			std::string name_; 
			int zorder_;
			Color namebox_text_color_;
			std::string textbox_path_;
			std::string namebox_path_; 
			std::string transform_name_;
			bool is_speaking_;
			bool is_visible_;
		};
		Editableproperties properties_;
		Transform transform_;
		std::unique_ptr<Image> character_; //TODO : juste pour tester => truc similaire à MPT par la suite

	private:
		//SDL_Rect initial_rect_;
};

