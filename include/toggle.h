#pragma once

#include "ui.h"
#include "image.h"

class Toggle : public Ui
{
	public:
		Toggle(const std::string path_normal, const std::string path_selected, const std::string path_clicked, const std::string path_checked, const int x, const int y, bool is_checked, SDL_Renderer* renderer, std::function<void(Ui* ui)> callback_function);

		void on_pointer_up() override; //<=> on click (l'action se lance quand le clic est relaché)
		void draw(SDL_Renderer* renderer) override;
		void update(Uint64& time_step) override;

	protected:

	private:
		Image normal;
		Image selected;
		Image clicked;
		Image checked; //TODO : renommer ??*/

		bool is_checked;
};

