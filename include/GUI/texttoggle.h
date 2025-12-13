#pragma once

#include "GUI/checkable.h"
#include "text.h"

enum class TextToggleKind
{
	NORMAL,
	ON_TEXTBOX
};

class TextToggle : public Checkable
{
	public:
		TextToggle(const std::string_view text, const int x, const int y, const bool is_checked, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function, const TextToggleKind texttoggle_kind = TextToggleKind::NORMAL);

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		SDL_Rect get_rect() const override;

		Text text_;

	private:
		static const unsigned int index_rect_texttoggle_;
};

