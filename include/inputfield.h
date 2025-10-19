#pragma once

#include "ui.h"
#include "text.h"

class Inputfield : public Ui
{
	public:
		Inputfield(std::string path, SDL_Color color, int x, int y, SDL_Renderer* renderer, void(*function_ptr)(std::string text));
		~Inputfield();

		SDL_Texture* image;

		SDL_Color color;

		Text text;
		Text text_caret;
		Text text_placeholder;

		bool is_editing; //TODO : renommer en is_editing
		bool is_writing;
		int character_limit;

		void(*function_ptr)(std::string text);

		//TODO : ajouter les override
		void on_pointer_down();
		void on_pointer_enter();
		void on_pointer_exit();
		void on_pointer_up(); //<=> on click (l'action se lance quand le clic est relaché)

		void on_input(SDL_Event e);

		//TODO : abstraire le fait que ça soit une touche du clavier ou la manette dans les if
		void on_key_pressed(SDL_Event e);
		void on_key_released(SDL_Event e);

		void handle_events(SDL_Event e) override;
		void draw(SDL_Renderer* renderer);
		void update(Uint32& timeStep);

	protected:

	private:
};

