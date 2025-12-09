#pragma once

#include "image.h"
#include "GUI/textbutton.h"

#include <string>

class ConfirmationPopUp : public Ui
{
	public:
		ConfirmationPopUp(const std::string_view text, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		std::vector<Ui*> get_navigation_nodes() override;

		bool is_confirmationpopup_visible_;

		TextButton yes_;
		TextButton no_;

	private:
		Text text_;
		Image frame_;
		Image background_;
};

