#pragma once

#include "transform.h"

class Character
{
	public:
		Character(const std::string_view character_name, const std::string_view character_path, sdl::Renderer& renderer);
		Character(const std::string_view character_name, const std::string_view character_path, sdl::Renderer& renderer, const std::string_view textbox_path, const std::string_view namebox_path);

		void set_transform(const std::string transform_name);
		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update();

		struct Editableproperties
		{
			std::string name_; //garder le type std::string pour permettre au joueur de taper son nom au clavier
			int zorder_;
			std::string textbox_path_;
			std::string namebox_path_;
			Transform transform_;
		};

		static int counter_;
		Image character_; //TODO : juste pour tester => truc similaire à MPT par la suite
		Editableproperties properties_;
		std::string unique_id_;

		bool is_speaking_;
		bool is_visible_;

	private:
		//SDL_Rect initial_rect_;
};

