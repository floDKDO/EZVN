#include "GUI/ui.h"
#include "constants.h"

#include <iostream>

//TODO : parfois un bug avec la touche Entrée au lancement du programme

Ui::Ui(sdl::Renderer& renderer)
	: select_on_up_(nullptr), select_on_down_(nullptr), select_on_left_(nullptr), select_on_right_(nullptr),
	state_(State::NORMAL), last_time_(0),
	pointer_on_ui_when_pointer_up_(true), has_keyboard_focus_(false), mouse_entered_(false), mouse_was_on_ui_before_drag_(false),
	wants_text_input_(false), renderer_(renderer),
	callback_function_(nullptr)
{}

void Ui::on_pointer_up(PointerEventData pointer_event_data) 
{
	state_ = State::SELECTED;
	callback_function_(this);
	on_pointer_up_hook_end(pointer_event_data);
}

void Ui::on_pointer_down(PointerEventData pointer_event_data)
{
	state_ = State::PRESSED;
	on_pointer_down_hook_end(pointer_event_data);
}

void Ui::on_pointer_enter(PointerEventData pointer_event_data)
{
	state_ = State::SELECTED;
	on_pointer_enter_hook_end(pointer_event_data);
}

void Ui::on_pointer_exit(PointerEventData pointer_event_data)
{
	if(state_ == State::PRESSED && pointer_on_ui_when_pointer_up_)
	{
		state_ = State::SELECTED;
	}

	on_pointer_exit_hook_end(pointer_event_data); 
}

void Ui::on_up_pressed()
{
	on_up_pressed_hook_end();
}

void Ui::on_down_pressed()
{
	on_down_pressed_hook_end();
}

void Ui::on_left_pressed()
{
	on_left_pressed_hook_end();
}

void Ui::on_right_pressed()
{
	on_right_pressed_hook_end();
}

void Ui::on_enter_pressed()
{
	state_ = State::PRESSED;
	on_enter_pressed_hook_end();
}

void Ui::on_up_released()
{
	on_up_released_hook_end();
}

void Ui::on_down_released()
{
	on_down_released_hook_end();
}

void Ui::on_left_released()
{
	on_left_released_hook_end();
}

void Ui::on_right_released()
{
	on_right_released_hook_end();
}

void Ui::on_enter_released()
{
	state_ = State::SELECTED;
	callback_function_(this);
	on_enter_released_hook_end();
}

std::vector<Ui*> Ui::get_navigation_nodes()
{
	return {this};
}