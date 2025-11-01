#pragma once

#include "ui.h"
#include "text.h"
#include "image.h"

class Inputfield : public Ui
{
	public:
		Inputfield(const std::string path, const SDL_Color color_normal, unsigned int character_limit, const int x, const int y, SDL_Renderer* renderer, std::function<void(Ui* ui)> callback_function);

		void on_pointer_up(bool pointer_on_ui_when_pointer_up) override; //<=> on click (l'action se lance quand le clic est relaché)

		void on_left_pressed() override;
		void on_right_pressed() override;
		void on_enter_pressed() override;
		void on_backspace_pressed();
		void on_delete_pressed();
		void on_input_pressed(const SDL_Event& e) override;

		void on_input_released(const SDL_Event& e) override;
		void handle_events(const SDL_Event& e) override;
		void draw(SDL_Renderer* renderer) override;
		void update(Uint64& time_step) override;

		void on_typing(const SDL_Event& e);
		void set_character_limit(unsigned int character_limit);

		Text text;

	protected:

	private:
		Image normal;
		SDL_Color color_normal;

		unsigned int character_limit;

		Text text_caret;
		Text text_placeholder;

		bool is_editing;
		bool is_writing;
		
		unsigned int index_caret;
		int offset_caret; 
};

