#include "GUI/text_button.h"
#include "constants.h"
//#include "GUI/confirmation_popup.h"

#include <iostream>

const unsigned int TextButton::index_rect_textbutton_ = 0;

TextButton::TextButton(std::string_view text, int x, int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function, Kind textbutton_kind)
	: Ui(renderer), 
	  text_(text, constants::textbutton_normal_color_, constants::textbutton_font_, textbutton_kind == Kind::ON_TEXTBOX ? constants::textbox_ui_elements_text_size_ : textbutton_kind == Kind::ON_FRAME ? constants::confirmationpopup_text_size_ : constants::textbutton_text_size_, x, y, renderer)
{
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = true;
}

/*TextButton::TextButton(std::string_view text, int x, int y, sdl::Renderer& renderer, std::string_view text_popup, std::function<void(Ui* ui)> callback_function, TextButtonKind textbutton_kind)
	: Ui(renderer),
	text_(text, constants::textbutton_normal_color_, constants::textbutton_font_, textbutton_kind == TextButtonKind::ON_TEXTBOX ? constants::textbox_ui_elements_text_size_ : textbutton_kind == TextButtonKind::ON_FRAME ? constants::confirmationpopup_text_size_ : constants::textbutton_text_size_, x, y, renderer),
	confirmationpopup_(std::make_unique<ConfirmationPopUp>(text_popup, renderer, callback_function))
{
	callback_function_ = [&]([[maybe_unused]] Ui* ui)
	{
		confirmationpopup_->is_confirmationpopup_visible_ = true; 
		//Ui::is_pop_up_visible_ = true; 
		state_ = State::NORMAL;  
		confirmationpopup_->no_.state_ = State::SELECTED; 
	};

	pointer_on_ui_when_pointer_up_ = true;
}*/

void TextButton::draw(sdl::Renderer& renderer)
{
	text_.draw(renderer);
	/*if(confirmationpopup_)
	{
		confirmationpopup_->draw(renderer);
	}*/
}

void TextButton::update()
{
	text_.update(); //doit être placé en premier pour que le premier UI SELECTED dans un menu ait la bonne couleur

	if(state_ == State::NORMAL)
	{
		if(text_.color_.r_ != constants::textbutton_normal_color_.r_ 
		|| text_.color_.g_ != constants::textbutton_normal_color_.g_ 
		|| text_.color_.b_ != constants::textbutton_normal_color_.b_ 
		|| text_.color_.a_ != constants::textbutton_normal_color_.a_)
		{
			text_.change_color(constants::textbutton_normal_color_);
		}
	}
	else if(state_ == State::SELECTED)
	{
		if(text_.color_.r_ != constants::textbutton_selected_color_.r_
		|| text_.color_.g_ != constants::textbutton_selected_color_.g_
		|| text_.color_.b_ != constants::textbutton_selected_color_.b_
		|| text_.color_.a_ != constants::textbutton_selected_color_.a_)
		{
			text_.change_color(constants::textbutton_selected_color_);
		}
	}
	else if(state_ == State::PRESSED)
	{
		if(text_.color_.r_ != constants::textbutton_pressed_color_.r_
		|| text_.color_.g_ != constants::textbutton_pressed_color_.g_
		|| text_.color_.b_ != constants::textbutton_pressed_color_.b_
		|| text_.color_.a_ != constants::textbutton_pressed_color_.a_)
		{
			text_.change_color(constants::textbutton_pressed_color_);
		}
	}
	
	/*if(confirmationpopup_)
	{
		confirmationpopup_->update();
	}*/
}

SDL_Rect TextButton::get_rect() const
{
	return text_.position_;
}

void TextButton::set_rect(SDL_Rect rect)
{
	text_.position_ = rect;
}