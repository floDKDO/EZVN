#pragma once

#include "image.h"
#include "text.h"

//TODO : se baser sur le code de boiteDeDialogue mais ne pas découper les string selon les \n, les laisser aller à la ligne normalement
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

