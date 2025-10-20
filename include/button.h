#pragma once

#include "ui.h"

class Button : public Ui
{
	public:
		Button(const std::string path_normal, const std::string path_selected, const std::string path_clicked, const int x, const int y, SDL_Renderer* renderer, void(*function_ptr)(void)); //TODO : au moins une image pour l'état NORMAL, le reste est optionnel
		~Button();

		SDL_Texture* normal_image;
		SDL_Texture* selected_image;
		SDL_Texture* clicked_image;

		void(*function_ptr)(void);

		void on_pointer_down() override;
		void on_pointer_enter() override;
		void on_pointer_exit() override;
		void on_pointer_up() override; //<=> on click (l'action se lance quand le clic est relaché)

		//TODO : abstraire le fait que ça soit une touche du clavier ou la manette dans les if
		void on_key_pressed(const SDL_Event& e) override;
		void on_key_released(const SDL_Event& e) override;

		void draw(SDL_Renderer* renderer) override;
		void update(Uint32& timeStep) override;

	protected:

	private:

};

