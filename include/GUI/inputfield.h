#pragma once

#include "GUI/ui_widget.h"
#include "text.h"

class Inputfield : public UiWidget
{
	public:
		Inputfield(std::string_view text_placeholder, unsigned int character_limit, int x, int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		void quit_editing();

		void on_pointer_up_hook_end(PointerEventData pointer_event_data) override;

		void on_left_pressed() override;
		void on_right_pressed() override;
		void on_enter_pressed_hook_end() override;
		void on_backspace_pressed() override;
		void on_delete_pressed() override;
		void draw(sdl::Renderer& renderer) override;
		void update() override;
		void change_position(int x, int y) override;

		void on_typing(std::string_view text) override;
		void set_character_limit(unsigned int character_limit);

	private:
		Text text_;

		SDL_Rect container_;
		SDL_Rect container_outline_;

		unsigned int character_limit_;

		Text text_caret_;
		Text text_placeholder_;

		Uint64 last_blink_time_;
		Uint64 last_input_time_;
		bool is_caret_visible_;
		
		size_t index_caret_;
		int offset_caret_; 
};