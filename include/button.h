#pragma once

#include "ui.h"

class Button : public Ui
{
	public:
		Button(std::string path_normal, std::string path_selected, std::string path_clicked, int x, int y, SDL_Renderer* renderer, void(*function_ptr)(void)); //TODO : au moins une image pour l'état NORMAL, le reste est optionnel
		~Button();

		SDL_Texture* normal_image;
		SDL_Texture* selected_image;
		SDL_Texture* clicked_image;

		void(*function_ptr)(void);

		//TODO : ajouter les override
		void on_pointer_down();
		void on_pointer_enter();
		void on_pointer_exit();
		void on_pointer_up(); //<=> on click (l'action se lance quand le clic est relaché)

		//TODO : abstraire le fait que ça soit une touche du clavier ou la manette dans les if
		void on_key_pressed(SDL_Event e);
		void on_key_released(SDL_Event e);

		void draw(SDL_Renderer* renderer);
		void update(Uint32& timeStep);

	protected:

	private:

};

