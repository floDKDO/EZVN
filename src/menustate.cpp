#include "menustate.h"

MenuState::MenuState(const std::string background_path, std::vector<std::unique_ptr<Ui>> ui_elements, sdl::Renderer& renderer)
	: ui_manager_(std::move(ui_elements)), background_(background_path, 0, 0, renderer)
{}

void MenuState::handle_events(const SDL_Event& e)
{
	ui_manager_.handle_events(e);
}

void MenuState::draw(sdl::Renderer& renderer)
{
	background_.draw(renderer);
	ui_manager_.draw(renderer);
}

void MenuState::update()
{
	ui_manager_.update();
}