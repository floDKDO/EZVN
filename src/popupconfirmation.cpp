#include "popupconfirmation.h"
#include "constants.h"

PopUpConfirmation::PopUpConfirmation(const std::string text, std::vector<std::unique_ptr<Ui>> ui_elements, sdl::Renderer& renderer)
	: text_(text, constants::popupconfirmation_text_color_, constants::popupconfirmation_font_, constants::popupconfirmation_text_size_, 0, 0, renderer), 
	frame_(constants::popupconfirmation_frame_, 0, 0, renderer), ui_manager_(std::move(ui_elements))
{
	frame_.set_center();
}

void PopUpConfirmation::handle_events(const SDL_Event& e)
{
	ui_manager_.handle_events(e);
}

void PopUpConfirmation::draw(sdl::Renderer& renderer)
{
	frame_.draw(renderer);
	text_.draw(renderer);
	ui_manager_.draw(renderer);
}

void PopUpConfirmation::update()
{
	text_.update();
	ui_manager_.update();
}
