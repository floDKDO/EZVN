#pragma once

#include "gamestate.h"
#include "image.h"
#include "popupconfirmation.h"
#include "GUI/uimanager.h"

class MenuState : public GameState
{
	public:
		MenuState(const std::string background_path, std::vector<std::unique_ptr<Ui>> ui_elements, sdl::Renderer& renderer);
		MenuState(const std::string background_path, std::vector<std::unique_ptr<Ui>> ui_elements_menu, const std::string popup_text, std::vector<std::unique_ptr<Ui>> ui_elements_popup, sdl::Renderer& renderer);
		void handle_events(const SDL_Event& e) override;
		void draw(sdl::Renderer& renderer) override;
		void update() override;

		bool is_popup_confirmation_visible_;

	protected:

	private:
		UiManager ui_manager_;
		Image background_;
		std::unique_ptr<PopUpConfirmation> popup_confirmation_;
	
};

