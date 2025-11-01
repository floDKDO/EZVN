#pragma once

#include "image.h"
#include "text.h"

class Textbox
{
	public:
		Textbox(SDL_Color text_color, SDL_Renderer* renderer);

		void draw(SDL_Renderer* renderer);
		void update(Uint64& time_step);

		Image namebox; //TODO : ne pas l'afficher si le personnage pense / narrateur parle
		Image textbox;
		Text text;

	protected:

	private:

};

