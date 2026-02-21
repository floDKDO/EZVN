#include "GUI/button.h"
#include "constants.h"

const unsigned int Button::index_rect_button_ = 0;

Button::Button(std::string_view text, int x, int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: UiWidget(renderer), 
	text_(text, constants::button_text_color_, constants::button_font_, constants::button_text_size_, x, y, renderer),
	normal_(constants::button_normal_, x, y, renderer), 
	selected_(constants::button_selected_, x, y, renderer),
	pressed_(constants::button_pressed_, x, y, renderer)
{
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = true;

	text_.position_.x = normal_.position_.x + normal_.position_.w / 2 - text_.get_width_text() / 2;

	rect_ = normal_.position_;
}

//TODO : constructor delegation
Button::Button(std::string_view path_normal, std::string_view text, int x, int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: UiWidget(renderer), 
	text_(text, constants::button_text_color_, constants::button_font_, constants::button_text_size_, x, y, renderer),
	normal_(path_normal, x, y, renderer),
	selected_(path_normal, x, y, renderer),
	pressed_(path_normal, x, y, renderer)
{
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = true;

	text_.position_.x = normal_.position_.x + normal_.position_.w / 2 - text_.get_width_text() / 2;

	rect_ = normal_.position_;
}

void Button::draw(sdl::Renderer& renderer) 
{
	if(state_ == State::NORMAL)
	{
		normal_.draw(renderer);
	}
	else if(state_ == State::SELECTED)
	{
		selected_.draw(renderer);
	}
	else if(state_ == State::PRESSED)
	{
		pressed_.draw(renderer);
	}

	text_.draw(renderer);
}

void Button::update()
{
	text_.update();
	normal_.update();
	selected_.update();
	pressed_.update();
}

void Button::change_position(int x, int y)
{
	text_.position_.x = x;
	text_.position_.y = y;

	normal_.position_.x = x;
	normal_.position_.y = y;

	selected_.position_.x = x;
	selected_.position_.y = y;

	pressed_.position_.x = x;
	pressed_.position_.y = y;

	rect_ = normal_.position_;
}