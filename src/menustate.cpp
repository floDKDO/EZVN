#include "menustate.h"

MenuState::MenuState(const std::string background_path, std::vector<std::unique_ptr<Ui>> ui_elements, Ui* ui_selected, sdl::Renderer& renderer)
	: background_(background_path, 0, 0, renderer), ui_manager_(std::move(ui_elements), ui_selected), is_popup_confirmation_visible_(false)
{

}

MenuState::MenuState(const std::string background_path, std::vector<std::unique_ptr<Ui>> ui_elements_menu, Ui* ui_selected_menu, const std::string popup_text, std::vector<std::unique_ptr<Ui>> ui_elements_popup, Ui* ui_selected_popup, sdl::Renderer& renderer)
	: background_(background_path, 0, 0, renderer), ui_manager_(std::move(ui_elements_menu), ui_selected_menu), is_popup_confirmation_visible_(false)
{
	popup_confirmation_ = std::make_unique<PopUpConfirmation>(popup_text, std::move(ui_elements_popup), ui_selected_popup, renderer);
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

void MenuState::update(Uint64 time_step)
{
	ui_manager_.update(time_step);
	if(is_popup_confirmation_visible_)
	{
		popup_confirmation_->update(time_step);
	}
}