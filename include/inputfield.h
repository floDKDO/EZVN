#pragma once

#include "ui.h"
#include "text.h"

class Inputfield : public Ui
{
	public:
		Inputfield(const std::string path, const SDL_Color color, const int x, const int y, SDL_Renderer* renderer, void(*function_ptr)(std::string text));
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

		void on_pointer_down() override;
		void on_pointer_enter() override;
		void on_pointer_exit() override;
		void on_pointer_up() override; //<=> on click (l'action se lance quand le clic est relaché)

		void on_input(const SDL_Event& e);

		//TODO : abstraire le fait que ça soit une touche du clavier ou la manette dans les if
		void on_key_pressed(const SDL_Event& e) override;
		void on_key_released(const SDL_Event& e) override;

		void handle_events(const SDL_Event& e) override;
		void draw(SDL_Renderer* renderer) override;
		void update(Uint32& timeStep) override;

	protected:

	private:
};

