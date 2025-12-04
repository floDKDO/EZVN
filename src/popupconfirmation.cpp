#include "popupconfirmation.h"
#include "constants.h"

PopUpConfirmation::PopUpConfirmation(const std::string text, sdl::Renderer& renderer, std::array<std::function<void(Ui* ui)>, 2> callback_functions)
	: Ui(renderer), is_popupconfirmation_visible_(false),
	text_(text, constants::popupconfirmation_text_color_, constants::popupconfirmation_font_, constants::popupconfirmation_text_size_, 0, 0, renderer, constants::popupconfirmation_width_ - constants::popupconfirmation_text_x_delta_),
	frame_(constants::popupconfirmation_frame_, 0, 0, renderer),
	background_(constants::popupconfirmation_background, 0, 0, renderer),
	yes_("Yes", 300, 500, renderer_, callback_functions[0], TextButtonKind::ON_FRAME), //TODO : hardcodé
	no_("No", 600, 500, renderer_, callback_functions[1], TextButtonKind::ON_FRAME) //TODO : hardcodé
{
	frame_.set_center();
	text_.set_center();
	text_.position_.x -= constants::popupconfirmation_text_x_delta_;
}

void PopUpConfirmation::draw(sdl::Renderer& renderer)
{
	if(is_popupconfirmation_visible_)
	{
		background_.draw(renderer);
		frame_.draw(renderer);
		text_.draw(renderer);
		yes_.draw(renderer);
		no_.draw(renderer);
	}
}

void PopUpConfirmation::update()
{
	if(is_popupconfirmation_visible_)
	{
		text_.update();
		yes_.update();
		no_.update();
	}
}
