#pragma once

#include "transform.h"

class Character
{
	public:
		Character(const std::string name, const std::string character_path, sdl::Renderer& renderer, const int zorder = 0);
		//Character(std::string name, std::string character_path, sdl::Renderer& renderer, std::string textbox_path, int zorder = 0); //TODO : valeur par défaut au cas-où on ne souhaite pas avoir une textbox propre à chaque perso

		void set_transform(const TransformName transform_name);
		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update();

		Image character_; //TODO : juste pour tester => truc similaire à MPT par la suite
		std::string name_;
		//Image textbox_; //TODO : valeur par défaut au cas-où on ne souhaite pas avoir une textbox propre à chaque perso

		bool is_speaking_;

	protected:


	private:
		//TODO : focus automatique
		Transform t_;
		//SDL_Rect initial_rect_;
};

