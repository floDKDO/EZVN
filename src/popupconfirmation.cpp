#include "popupconfirmation.h"

PopUpConfirmation::PopUpConfirmation(const std::string text, std::vector<std::unique_ptr<Ui>> ui_elements, Ui* ui_selected, sdl::Renderer& renderer)
	: text_(text, {255, 255, 255, 255}, "fonts/Aller_Rg.ttf", 50, 0, 0, renderer), frame_("img/GUI/frame.png", 0, 0, renderer), ui_manager_(std::move(ui_elements), ui_selected)
{
	frame_.set_center(0);
}

void PopUpConfirmation::handle_events(const SDL_Event& e)
{
	ui_manager_.handle_events(e);
}

void PopUpConfirmation::draw(sdl::Renderer& renderer)
{
	text_.draw(renderer);
	frame_.draw(renderer);
	ui_manager_.draw(renderer);
}

void PopUpConfirmation::update(Uint64 time_step)
{
	text_.update(time_step);
	ui_manager_.update(time_step);
}
