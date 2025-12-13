#include "GUI/textbutton.h"
#include "constants.h"
#include "GUI/confirmationpopup.h"

#include <iostream>

const unsigned int TextButton::index_rect_textbutton_ = 0;

TextButton::TextButton(const std::string_view text, const int x, const int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function, const TextButtonKind textbutton_kind)
	: Ui(renderer), 
	  text_(text, constants::textbutton_text_color_, constants::textbutton_font_, textbutton_kind == TextButtonKind::ON_TEXTBOX ? constants::textbox_ui_elements_text_size_ : textbutton_kind == TextButtonKind::ON_FRAME ? constants::confirmationpopup_text_size_ : constants::textbutton_text_size_, x, y, renderer)
{
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = true;
}

TextButton::TextButton(const std::string_view text, const int x, const int y, sdl::Renderer& renderer, const std::string_view text_popup, std::function<void(Ui* ui)> callback_function, const TextButtonKind textbutton_kind)
	: Ui(renderer),
	text_(text, constants::textbutton_text_color_, constants::textbutton_font_, textbutton_kind == TextButtonKind::ON_TEXTBOX ? constants::textbox_ui_elements_text_size_ : textbutton_kind == TextButtonKind::ON_FRAME ? constants::confirmationpopup_text_size_ : constants::textbutton_text_size_, x, y, renderer),
	confirmationpopup_(std::make_unique<ConfirmationPopUp>(text_popup, renderer, callback_function))
{
	callback_function_ = [&](Ui* ui)
	{
		(void)ui; 
		confirmationpopup_->is_confirmationpopup_visible_ = true; 
		Ui::is_pop_up_visible_ = true; 
		state_ = State::NORMAL;  
		confirmationpopup_->no_.state_ = State::SELECTED; 
	};

	pointer_on_ui_when_pointer_up_ = true;
}

void TextButton::draw(sdl::Renderer& renderer)
{
	text_.draw(renderer);
	if(confirmationpopup_)
	{
		confirmationpopup_->draw(renderer);
	}
}

void TextButton::update()
{
	if(state_ == State::NORMAL)
		text_.change_color(constants::textbutton_normal_color_);
	else if(state_ == State::SELECTED)
		text_.change_color(constants::textbutton_selected_color_);
	else if(state_ == State::CLICKED)
		text_.change_color(constants::textbutton_clicked_color_);
	
	if(confirmationpopup_)
	{
		confirmationpopup_->update();
	}
}

SDL_Rect TextButton::get_rect() const
{
	return text_.position_;
}