#pragma once

#include "transform.h"

class Character
{
	public:
		Character(const std::string_view name, const std::string_view character_path, sdl::Renderer& renderer, const int zorder = 0);
		Character(const std::string_view name, const std::string_view character_path, sdl::Renderer& renderer, const std::string_view textbox_path, const std::string_view namebox_path, const int zorder = 0);

		void set_transform(const TransformName transform_name);
		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update();

		Image character_; //TODO : juste pour tester => truc similaire à MPT par la suite
		std::string name_; //garder le type std::string pour permettre au joueur de taper son nom au clavier

		std::string textbox_path_; 
		std::string namebox_path_;

		bool is_speaking_;
		bool is_visible_;

		//TODO : focus automatique
		Transform t_;

	private:
		//SDL_Rect initial_rect_;
};

