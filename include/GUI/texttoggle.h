#pragma once

#include "GUI/checkable.h"
#include "text.h"

class TextToggleGroup;

class TextToggle : public Checkable
{
	//friend class TextToggleGroup;

	public:
		TextToggle(const std::string text, SDL_Color color_unchecked, SDL_Color color_selected, SDL_Color color_checked, const int x, const int y, bool is_checked, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		void draw(sdl::Renderer& renderer) override;
		void update(Uint64 time_step) override;
		SDL_Rect get_rect() const override;


	protected:

	private:
		Text text_;

		SDL_Color color_unchecked_;
		SDL_Color color_selected_;
		SDL_Color color_checked_;

		static const unsigned int INDEX_RECT_TEXTTOGGLE_ = 0;

		TextToggleGroup* texttoggle_group_ = nullptr;
};

