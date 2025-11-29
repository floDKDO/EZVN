#pragma once

#include "transform.h"

class Character
{
	public:
		Character(std::string name, std::string character_path, std::string textbox_path, sdl::Renderer& renderer, int zorder=0);

		void set_transform(TransformName transform_name);
		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update(Uint64 time_step);

		std::string name_;

		//TODO : focus automatique
		Transform t_;

		Image character_; //TODO : juste pour tester => truc similaire à MPT par la suite
		Image textbox_; //TODO : valeur par défaut au cas-où on ne souhaite pas avoir une textbox propre à chaque perso

		SDL_Rect initial_rect_;

		bool is_speaking_;

		int zorder_; //TODO : redondant avec le zorder de character_

	protected:


	private:

};

