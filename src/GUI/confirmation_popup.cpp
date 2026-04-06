#include "GUI/confirmation_popup.h"
#include "constants.h"

#include <iostream>

ConfirmationPopUp::ConfirmationPopUp(std::string_view message, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: UiWidget(renderer), 
	yes_("Yes", 0, 0, renderer_, callback_function, TextButton::Kind::ON_FRAME), 
	no_("No", 0, 0, renderer_, 
	[&]([[maybe_unused]] Ui* ui)
	{
		std::cout << "Pressed No!" << std::endl;
		is_visible_ = false;  
	}, 
	TextButton::Kind::ON_FRAME), 
	message_(message, constants::confirmationpopup_text_color_, constants::confirmationpopup_font_, constants::confirmationpopup_text_size_, 0, 0, renderer, false, constants::confirmationpopup_width_),
	frame_(constants::confirmationpopup_frame_, 0, 0, renderer),
	background_(constants::confirmationpopup_background_, 0, 0, renderer)
{
	frame_.set_center();

	yes_.change_position(frame_.position_.x + constants::confirmationpopup_buttons_x_delta_, frame_.position_.y + constants::confirmationpopup_buttons_y_delta_); 
	no_.change_position(frame_.position_.x + frame_.position_.w - no_.rect_.w - constants::confirmationpopup_buttons_x_delta_, frame_.position_.y + constants::confirmationpopup_buttons_y_delta_); 

	message_.set_center();
	message_.position_.y = frame_.position_.y + constants::confirmationpopup_text_y_delta_; 
	rect_ = frame_.position_;
}

void ConfirmationPopUp::change_callback(std::function<void(Ui* ui)> callback_function)
{
	yes_.callback_function_ = callback_function;
}

void ConfirmationPopUp::change_message(std::string_view message)
{
	message_.change_text(message);
	message_.set_center();
	message_.position_.y = frame_.position_.y + constants::confirmationpopup_text_y_delta_;
}

void ConfirmationPopUp::draw(sdl::Renderer& renderer)
{
	if(is_visible_)
	{
		background_.draw(renderer);
		frame_.draw(renderer);
		message_.draw(renderer);
		yes_.draw(renderer);
		no_.draw(renderer);
	}
}

void ConfirmationPopUp::update()
{
	if(is_visible_)
	{
		message_.update();
		yes_.update();
		no_.update();
		frame_.update();
		background_.update();
	}
}

std::vector<UiWidget*> ConfirmationPopUp::get_navigation_nodes()
{
	std::vector<UiWidget*> vector;
	vector.push_back(&yes_);
	vector.push_back(&no_);
	return vector;
}

void ConfirmationPopUp::change_position([[maybe_unused]] int x, [[maybe_unused]] int y)
{
	//TODO : ne fait rien ??
}