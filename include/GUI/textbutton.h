#pragma once

#include "GUI/ui.h"
#include "text.h"

#include <memory>

//class ConfirmationPopUp;

enum class TextButtonKind
{
	NORMAL,
	ON_TEXTBOX,
	ON_FRAME
};

class TextButton : public Ui
{
	public:
		TextButton(const std::string_view text, const int x, const int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function, const TextButtonKind textbutton_kind = TextButtonKind::NORMAL);
		//TextButton(const std::string_view text, const int x, const int y, sdl::Renderer& renderer, const std::string_view text_popup, std::function<void(Ui* ui)> callback_function, const TextButtonKind textbutton_kind = TextButtonKind::NORMAL);

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		SDL_Rect get_rect() const override;

	private:
		static const unsigned int index_rect_textbutton_;

	public:
		Text text_;
		//std::unique_ptr<ConfirmationPopUp> confirmationpopup_;
};

