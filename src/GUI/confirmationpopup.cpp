#include "GUI/confirmationpopup.h"
#include "constants.h"

#include <iostream>

ConfirmationPopUp::ConfirmationPopUp(std::string_view text, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: Ui(renderer), is_confirmationpopup_visible_(false),
	yes_("Yes", 200, 500, renderer_, callback_function, TextButtonKind::ON_FRAME), //TODO : hardcodé
	no_("No", 500, 500, renderer_, 
	[&]([[maybe_unused]] Ui* ui)
	{
		is_confirmationpopup_visible_ = false; 
		//Ui::is_pop_up_visible_ = false; 
	}, 
	TextButtonKind::ON_FRAME), //TODO : hardcodé
	text_(text, constants::confirmationpopup_text_color_, constants::confirmationpopup_font_, constants::confirmationpopup_text_size_, 0, 0, renderer, false, constants::confirmationpopup_width_ - constants::confirmationpopup_text_x_delta_),
	frame_(constants::confirmationpopup_frame_, 0, 0, renderer),
	background_(constants::confirmationpopup_background_, 0, 0, renderer)
{
	frame_.set_center();
	text_.set_center();
	text_.position_.x -= constants::confirmationpopup_text_x_delta_;
}

void ConfirmationPopUp::draw(sdl::Renderer& renderer)
{
	if(is_confirmationpopup_visible_)
	{
		background_.draw(renderer);
		frame_.draw(renderer);
		text_.draw(renderer);
		yes_.draw(renderer);
		no_.draw(renderer);
	}
}

void ConfirmationPopUp::update()
{
	if(is_confirmationpopup_visible_)
	{
		text_.update();
		yes_.update();
		no_.update();
	}
}

std::vector<Ui*> ConfirmationPopUp::get_navigation_nodes()
{
	std::vector<Ui*> vector;	
	vector.push_back(&yes_);
	vector.push_back(&no_);
	return vector;
}