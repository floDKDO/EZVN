#include "GUI/button.h"

Button::Button(const std::string path_normal, const std::string path_selected, const std::string path_clicked, const int x, const int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function)
	: Ui(renderer), normal_(path_normal, x, y, renderer), selected_(path_selected, x, y, renderer), clicked_(path_clicked, x, y, renderer)
{
	callback_function_ = callback_function;
	pointer_on_ui_when_pointer_up_ = true;
	//renderer_ = renderer;
}

void Button::draw(sdl::Renderer& renderer) 
{
	if(state_ == State::NORMAL)
		normal_.draw(renderer);
	else if(state_ == State::SELECTED)
		selected_.draw(renderer);
	else if(state_ == State::CLICKED)
		clicked_.draw(renderer);
}

void Button::update(Uint64 time_step)
{
	(void)time_step;
}

SDL_Rect Button::get_rect() const
{
	return normal_.position_;
}