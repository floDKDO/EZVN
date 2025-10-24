#pragma once

#include "sound.h"

#include <SDL2/SDL.h>

enum class State
{
	NORMAL, 
	CLICKED, 
	SELECTED
};

class Ui
{
	public:
		virtual ~Ui();

		//TODO : laisser virtual ??
		virtual void on_pointer_up(); //<=> on click (l'action se lance quand le clic est relaché)
		void on_pointer_down();
		void on_pointer_enter();
		void on_pointer_exit();

		//TODO : abstraire le fait que ça soit une touche du clavier ou la manette dans les if
		virtual void on_key_pressed(const SDL_Event& e);
		virtual void on_key_released(const SDL_Event& e);

		virtual void on_button_pressed(const SDL_Event& e);
		virtual void on_button_released(const SDL_Event& e);

		virtual void handle_events(const SDL_Event& e);
		virtual void draw(SDL_Renderer* renderer) = 0;
		virtual void update(Uint64& time_step) = 0;

		bool is_mouse_on_ui() const;

		SDL_Rect position;

		Ui* select_on_up;
		Ui* select_on_down;
		Ui* select_on_left;
		Ui* select_on_right;

		State state;

		std::string name;

		static bool lock; 

	protected:
		Ui(); 

		

		//TODO : stocker en "variables globales" select_sound, click_sound et une police par défaut ?
		Sound select_sound;
		Sound click_sound;
		bool is_selected_sound_played;

		void(*callback_function)(Ui* ui);


	private:
		void select_new(Ui* ui);

};

