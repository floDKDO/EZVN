#pragma once

#include "ui.h"
#include "text.h"

class TextToggle : public Ui
{
	friend class TextToggleGroup;

	public:
		TextToggle(const std::string text, SDL_Color color_unchecked, SDL_Color color_selected, SDL_Color color_checked, const int x, const int y, bool is_checked, SDL_Renderer* renderer, std::function<void(Ui* ui)> callback_function);

		void on_pointer_up_hook_end() override;
		void on_enter_released_hook_end() override;
		void draw(SDL_Renderer* renderer) override;
		void update(Uint64 time_step) override;
		SDL_Rect get_rect() const override;

		bool is_checked;

	protected:

	private:
		Text text;

		SDL_Color color_unchecked;
		SDL_Color color_selected;
		SDL_Color color_checked;

		static const unsigned int INDEX_RECT_TEXTTOGGLE = 0;
};

