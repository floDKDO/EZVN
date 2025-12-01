#pragma once

#include "image.h"
#include "GUI/uimanager.h"
#include "GUI/textbutton.h"

#include <string>

class PopUpConfirmation
{
	public:
		PopUpConfirmation(const std::string text, std::vector<std::unique_ptr<Ui>> ui_elements, sdl::Renderer& renderer);

		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update();

	protected:

	private:
		Text text_;
		Image frame_;
		UiManager ui_manager_; //2 TextButtons
};

