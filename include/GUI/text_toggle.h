#pragma once

#include "GUI/checkable.h"
#include "text.h"

class TextToggle : public Checkable
{
	public:
		enum class Kind
		{
			NORMAL,
			ON_TEXTBOX
		};

		TextToggle(std::string_view text, int x, int y, bool is_checked, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function, Kind texttoggle_kind = Kind::NORMAL);

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		void change_position(int x, int y) override;

		Text text_;
};

