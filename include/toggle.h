#pragma once

#include "ui.h"
#include "image.h"

class Toggle : public Ui
{
	public:
		Toggle(const std::string path_normal, const std::string path_selected, const std::string path_clicked, const std::string path_checked, const int x, const int y, bool is_checked, SDL_Renderer* renderer, void(*callback_function)(Ui* ui));

		Image normal;
		Image selected;
		Image clicked;
		Image checked; //TODO : renommer ??*/

		bool is_checked;

		void on_pointer_down() override;
		void on_pointer_enter() override;
		void on_pointer_exit() override;
		void on_pointer_up() override; //<=> on click (l'action se lance quand le clic est relaché)

		//TODO : abstraire le fait que ça soit une touche du clavier ou la manette dans les if
		void on_key_pressed(const SDL_Event& e) override;
		void on_key_released(const SDL_Event& e) override;

		void draw(SDL_Renderer* renderer) override;
		void update(Uint64& timeStep) override;

	protected:

	private:
};

