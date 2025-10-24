#pragma once

#include "ui.h"
#include "image.h"

class Button : public Ui
{
	public:
		Button(const std::string path_normal, const std::string path_selected, const std::string path_clicked, const int x, const int y, SDL_Renderer* renderer, void(*callback_function)(Ui* ui)); 

		void draw(SDL_Renderer* renderer) override;
		void update(Uint64& time_step) override;

		//DEBUG
		static int num;

	protected:

	private:
		Image normal;
		Image selected;
		Image clicked;
};

