#include "menustate.h"

MenuState::MenuState(const std::string background_path, std::vector<std::unique_ptr<Ui>> ui_elements, sdl::Renderer& renderer)
	: is_popup_confirmation_visible_(false), ui_manager_(std::move(ui_elements)), background_(background_path, 0, 0, renderer)
{}

MenuState::MenuState(const std::string background_path, std::vector<std::unique_ptr<Ui>> ui_elements_menu, const std::string popup_text, std::vector<std::unique_ptr<Ui>> ui_elements_popup, sdl::Renderer& renderer)
	: is_popup_confirmation_visible_(false), ui_manager_(std::move(ui_elements_menu)), background_(background_path, 0, 0, renderer)
{
	popup_confirmation_ = std::make_unique<PopUpConfirmation>(popup_text, std::move(ui_elements_popup), renderer);
}

void MenuState::handle_events(const SDL_Event& e)
{
	ui_manager_.handle_events(e);
	if(is_popup_confirmation_visible_)
	{
		popup_confirmation_->handle_events(e);
	}
}

void MenuState::draw(sdl::Renderer& renderer)
{
	background_.draw(renderer);
	ui_manager_.draw(renderer);
	if(is_popup_confirmation_visible_)
	{
		popup_confirmation_->draw(renderer);
	}
}

void MenuState::update()
{
	ui_manager_.update();
	if(is_popup_confirmation_visible_)
	{
		popup_confirmation_->update();
	}
}