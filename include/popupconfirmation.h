#pragma once

#include "image.h"
#include "GUI/uimanager.h"
#include "GUI/textbutton.h"

#include <string>
#include <array>

class PopUpConfirmation : public Ui
{
	public:
		PopUpConfirmation(const std::string text, sdl::Renderer& renderer, std::array<std::function<void(Ui* ui)>, 2> callback_functions);

		void draw(sdl::Renderer& renderer) override;
		void update() override;

	private:
		bool is_popupconfirmation_visible_;

		Text text_;
		Image frame_;
		Image background_;
		TextButton yes_;
		TextButton no_;
};

