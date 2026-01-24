#pragma once

#include "image.h"
#include "GUI/text_button.h"

class ConfirmationPopUp : public Ui
{
	public:
		ConfirmationPopUp(std::string_view text, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

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

