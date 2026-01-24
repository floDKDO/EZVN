#pragma once

#include "GUI/ui.h"
#include "text.h"

//class ConfirmationPopUp;

class TextButton : public Ui
{
	public:
		enum class Kind
		{
			NORMAL,
			ON_TEXTBOX,
			ON_FRAME //ex : ConfirmationPopUp
		};

		TextButton(std::string_view text, int x, int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function, Kind textbutton_kind = Kind::NORMAL);
		//TextButton(std::string_view text, int x, int y, sdl::Renderer& renderer, std::string_view text_popup, std::function<void(Ui* ui)> callback_function, TextButtonKind textbutton_kind = TextButtonKind::NORMAL);

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		SDL_Rect get_rect() const override;

	private:
		static const unsigned int index_rect_textbutton_;

	public:
		Text text_;
		//std::unique_ptr<ConfirmationPopUp> confirmationpopup_;
};

