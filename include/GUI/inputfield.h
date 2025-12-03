#pragma once

#include "GUI/ui.h"
#include "text.h"
#include "image.h"

class Inputfield : public Ui
{
	public:
		Inputfield(const unsigned int character_limit, const int x, const int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		void on_pointer_up_hook_end() override;

		void on_left_pressed() override;
		void on_right_pressed() override;
		void on_enter_pressed_hook_end() override;
		void on_backspace_pressed();
		void on_delete_pressed();
		void on_input_pressed_hook_end(const SDL_Event& e) override;
		void on_input_released_hook_end(const SDL_Event& e) override;
		void handle_events_hook_end(const SDL_Event& e) override;
		void draw(sdl::Renderer& renderer) override;
		void update() override;
		SDL_Rect get_rect() const override;

		void on_typing(const SDL_Event& e);
		void set_character_limit(unsigned int character_limit);

	protected:

	private:
		Text text_;
		Image normal_;

		unsigned int character_limit_;

		Text text_caret_;
		Text text_placeholder_;

		bool is_editing_;
		bool is_writing_;
		
		unsigned int index_caret_;
		int offset_caret_; 

		static const unsigned int index_rect_inputfield_;
};

