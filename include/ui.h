#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "sound.h"

enum class State
{
	NORMAL,
	HOVERED,
	CLICKED, //TODO : SELECTED <=> HOVERED ?? Si oui, garder SELECTED
	SELECTED
};

class Ui
{
	public:
		State state;

		Ui* select_on_up;
		Ui* select_on_down;
		Ui* select_on_left;
		Ui* select_on_right;

		Sound select_sound;
		Sound click_sound;

		SDL_Rect position;

		virtual void on_pointer_down() = 0;
		virtual void on_pointer_enter() = 0;
		virtual void on_pointer_exit() = 0;
		virtual void on_pointer_up() = 0; //<=> on click (l'action se lance quand le clic est relaché)

		//TODO : abstraire le fait que ça soit une touche du clavier ou la manette dans les if
		virtual void on_key_pressed() = 0;
		virtual void on_key_released() = 0;

		virtual void draw(SDL_Renderer* renderer) = 0;
		virtual void handle_events(SDL_Event e); //TODO : garder virtual ?

		virtual bool is_mouse_on_ui();

	protected:
		Ui(); //TODO : pour initialiser les attributs communs à toutes les classes filles
		virtual ~Ui();


	private:

};

