#pragma once

#include "ui.h"
#include "text.h"

//TODO : classe enfant de Button ?? => je ne pense pas parce qu'il n'y a pas d'image
class TextButton : public Ui
{
	public:
		TextButton(std::string text, SDL_Color color_normal, SDL_Color color_selected, SDL_Color color_clicked, int x, int y, SDL_Renderer* renderer, void(*function_ptr)(void));

		Text text;

		SDL_Color color_normal;
		SDL_Color color_selected;
		SDL_Color color_clicked;

		void(*function_ptr)(void);

		//TODO : ajouter les override
		void on_pointer_down();
		void on_pointer_enter();
		void on_pointer_exit();
		void on_pointer_up(); //<=> on_click (l'action se lance quand le clic est relaché)

		//TODO : abstraire le fait que ça soit une touche du clavier ou la manette dans les if
		void on_key_pressed(SDL_Event e);
		void on_key_released(SDL_Event e);

		void draw(SDL_Renderer* renderer);
		void update(Uint32& timeStep);

	protected:

	private:

};

