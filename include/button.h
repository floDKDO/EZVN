#pragma once

#include "ui.h"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Button : public Ui
{
	public:
		Button(std::string path_normal, int x, int y, SDL_Renderer* renderer, void(*function_ptr)(void)); //TODO : au moins une image pour l'état NORMAL, le reste est optionnel

		SDL_Texture* normal_image;
		SDL_Texture* selected_image;
		SDL_Texture* clicked_image;

		void(*function_ptr)(void);

		void on_pointer_down();
		void on_pointer_enter();
		void on_pointer_exit();
		void on_pointer_up(); //<=> on click (l'action se lance quand le clic est relaché)

		//TODO : abstraire le fait que ça soit une touche du clavier ou la manette dans les if
		void on_key_pressed();
		void on_key_released();

		void draw(SDL_Renderer* renderer);

	protected:

	private:

};

