#pragma once

#include "gamestate.h"
#include "image.h"
#include "GUI/uimanager.h"

class MenuState : public GameState
{
	public:
		MenuState(const std::string background_path, std::vector<std::unique_ptr<Ui>> ui_elements, Ui* ui_selected, sdl::Renderer& renderer);
		void handle_events(const SDL_Event& e) override;
		void draw(sdl::Renderer& renderer) override;
		void update(Uint64 time_step) override;

	protected:

	private:
		UiManager ui_manager_;
		Image background_;
};

