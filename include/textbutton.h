#pragma once

#include "ui.h"
#include "text.h"

//TODO : classe enfant de Button ?? => je ne pense pas parce qu'il n'y a pas d'image
class TextButton : public Ui
{
	public:
		TextButton(const std::string text, const SDL_Color color_normal, const SDL_Color color_selected, const SDL_Color color_clicked, const int x, const int y, SDL_Renderer* renderer, void(*callback_function)(Ui* ui));

		Text text;

		SDL_Color color_normal;
		SDL_Color color_selected;
		SDL_Color color_clicked;

		void on_pointer_down() override;
		void on_pointer_enter() override;
		void on_pointer_exit() override;
		void on_pointer_up() override; //<=> on_click (l'action se lance quand le clic est relaché)

		//TODO : abstraire le fait que ça soit une touche du clavier ou la manette dans les if
		void on_key_pressed(const SDL_Event& e) override;
		void on_key_released(const SDL_Event& e) override;

		void draw(SDL_Renderer* renderer) override;
		void update(Uint64& timeStep) override;

	protected:

	private:

};

