#include "GUI/ui.h"
#include "constants.h"

#include <iostream>

int Ui::is_pop_up_visible_ = false;
bool Ui::is_mouse_left_button_held_down_ = false;
bool Ui::lock_ = true;


//TODO : parfois un bug avec la touche Entrée au lancement du programme

Ui::Ui(sdl::Renderer& renderer)
	: select_on_up_(nullptr), select_on_down_(nullptr), select_on_left_(nullptr), select_on_right_(nullptr),
	state_(State::NORMAL), last_time_(0),
	select_sound_(constants::sound_hover_), click_sound_(constants::sound_click_),
	pointer_on_ui_when_pointer_up_(true), mouse_entered_(false), mouse_was_on_ui_before_drag_(false),
	renderer_(renderer),
	callback_function_(nullptr)
{}

void Ui::select_new(Ui* ui)
{
	state_ = State::NORMAL;
	ui->state_ = State::SELECTED;
	select_sound_.play_sound();
}

void Ui::on_pointer_up() //TODO : mettre les if dans handle_events ??
{
	if((pointer_on_ui_when_pointer_up_ && is_mouse_on_ui()) //first condition: the callback function is called only if the pointer is on the UI when it is released/up
	|| !pointer_on_ui_when_pointer_up_) //second conditon: the callback function is called even if the pointer is not on the UI when it is released/up
	{
		state_ = State::SELECTED;
		callback_function_(this);
		click_sound_.play_sound();
		on_pointer_up_hook_end();
	}
	is_mouse_left_button_held_down_ = false;
	mouse_was_on_ui_before_drag_ = false;
}

void Ui::on_pointer_down() 
{
	is_mouse_left_button_held_down_ = true;
	mouse_was_on_ui_before_drag_ = true;
	state_ = State::CLICKED;
	on_pointer_down_hook_end();
}

void Ui::on_pointer_enter() 
{
	mouse_entered_ = true;
	if(state_ == State::NORMAL)
	{
		state_ = State::SELECTED;
		select_sound_.play_sound();
		on_pointer_enter_hook_end();
	}
}

void Ui::on_pointer_exit() 
{
	if(state_ == State::CLICKED && pointer_on_ui_when_pointer_up_)
	{
		state_ = State::SELECTED; 
	}
	on_pointer_exit_hook_end(); //TODO : dans le if ??
	mouse_entered_ = false;
}

void Ui::on_up_pressed()
{
	if(lock_ && state_ == State::SELECTED && select_on_up_ != nullptr)
	{
		select_new(select_on_up_);
		lock_ = false;
		on_up_pressed_hook_end();
	}
}

void Ui::on_down_pressed()
{
	if(lock_ && state_ == State::SELECTED && select_on_down_ != nullptr)
	{
		select_new(select_on_down_);
		lock_ = false;
		on_down_pressed_hook_end();
	}
}

void Ui::on_left_pressed()
{
	if(lock_ && state_ == State::SELECTED && select_on_left_ != nullptr)
	{
		select_new(select_on_left_);
		lock_ = false;
		on_left_pressed_hook_end();
	}
}

void Ui::on_right_pressed()
{
	if(lock_ && state_ == State::SELECTED && select_on_right_ != nullptr)
	{
		select_new(select_on_right_);
		lock_ = false;
		on_right_pressed_hook_end();
	}
}

void Ui::on_enter_pressed()
{
	if(lock_ && state_ == State::SELECTED) //TODO : remettre les if des fonctions on_***_pressed dans on_input_pressed ??
	{
		state_ = State::CLICKED;
		lock_ = false;
		on_enter_pressed_hook_end();
	}
}

void Ui::on_input_pressed(const SDL_Event& e)
{
	if((e.type == SDL_CONTROLLERBUTTONDOWN && e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_UP))
	{
		on_up_pressed();
	}
	else if((e.type == SDL_CONTROLLERBUTTONDOWN && e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_DOWN))
	{
		on_down_pressed();
	}
	else if((e.type == SDL_CONTROLLERBUTTONDOWN && e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_LEFT))
	{
		on_left_pressed();
	}
	else if((e.type == SDL_CONTROLLERBUTTONDOWN && e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RIGHT))
	{
		on_right_pressed();
	}
	else if((e.type == SDL_CONTROLLERBUTTONDOWN && e.cbutton.button == SDL_CONTROLLER_BUTTON_A) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN))
	{
		on_enter_pressed();
	}
	on_input_pressed_hook_end(e);
}

void Ui::on_up_released()
{
	if(!lock_ && state_ == State::SELECTED)
	{
		lock_ = true;
		on_up_released_hook_end();
	}
}

void Ui::on_down_released()
{
	if(!lock_ && state_ == State::SELECTED)
	{
		lock_ = true;
		on_down_released_hook_end();
	}
}

void Ui::on_left_released()
{
	if(!lock_ && state_ == State::SELECTED)
	{
		lock_ = true;
		on_left_released_hook_end();
	}
}

void Ui::on_right_released()
{
	if(!lock_ && state_ == State::SELECTED)
	{
		lock_ = true;
		on_right_released_hook_end();
	}
}

void Ui::on_enter_released()
{
	if(state_ == State::CLICKED)
	{
		state_ = State::SELECTED;
		callback_function_(this);
		click_sound_.play_sound();
		lock_ = true;
		on_enter_released_hook_end();
	}
}

void Ui::on_input_released(const SDL_Event& e)
{
	if((e.type == SDL_CONTROLLERBUTTONUP && e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) || (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_UP))
	{
		on_up_released();
	}
	else if((e.type == SDL_CONTROLLERBUTTONUP && e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) || (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_DOWN))
	{
		on_down_released();
	}
	else if((e.type == SDL_CONTROLLERBUTTONUP && e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) || (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_LEFT))
	{
		on_left_released();
	}
	else if((e.type == SDL_CONTROLLERBUTTONUP && e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) || (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_RIGHT))
	{
		on_right_released();
	}
	else if((e.type == SDL_CONTROLLERBUTTONUP && e.cbutton.button == SDL_CONTROLLER_BUTTON_A) || (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_RETURN)) 
	{
		on_enter_released();
	}
	on_input_released_hook_end(e);
}

void Ui::handle_events(const SDL_Event& e)
{
	switch(e.type)
	{
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_KEYDOWN:
			on_input_pressed(e);
			break;

		case SDL_CONTROLLERBUTTONUP:
		case SDL_KEYUP:
			on_input_released(e);
			break;

		case SDL_MOUSEBUTTONDOWN:
			if(e.button.button == SDL_BUTTON_LEFT)
			{
				if(is_mouse_on_ui() && !is_mouse_left_button_held_down_)
				{
					on_pointer_down();
				}
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if(e.button.button == SDL_BUTTON_LEFT)
			{
				if(is_mouse_left_button_held_down_ && (state_ == State::SELECTED || state_ == State::CLICKED)) //cas "state == State::SELECTED" uniquement pour pouvoir bouger la poignée du Slider sans que la souris soit sur la poignée
				{
					on_pointer_up();
				}
			}
			break;

		case SDL_MOUSEMOTION: 
		{
			if(is_mouse_on_ui())
			{
				if(!mouse_entered_ && !is_mouse_left_button_held_down_)
				{
					on_pointer_enter();
				}
			}
			else
			{
				if(mouse_entered_)
				{
					on_pointer_exit();
				}
			}

			if(is_mouse_left_button_held_down_ && mouse_was_on_ui_before_drag_)
			{
				on_drag();
			}
		}
		break;

		default:
			break;
	}
	handle_events_hook_end(e);
}

void Ui::get_logical_mouse_position(int* logical_mouse_x, int* logical_mouse_y) const
{
	int real_mouse_x = 0, real_mouse_y = 0;
	SDL_GetMouseState(&real_mouse_x, &real_mouse_y);

	float temp_logical_mouse_x = 0, temp_logical_mouse_y = 0;
	SDL_RenderWindowToLogical(renderer_.fetch(), real_mouse_x, real_mouse_y, &temp_logical_mouse_x, &temp_logical_mouse_y);
	*logical_mouse_x = int(temp_logical_mouse_x);
	*logical_mouse_y = int(temp_logical_mouse_y);
}

bool Ui::is_mouse_on_ui() 
{
	int logical_mouse_x, logical_mouse_y;
	get_logical_mouse_position(&logical_mouse_x, &logical_mouse_y);

	std::vector<Ui*> all_ui = get_navigation_nodes();

	for(size_t i = 0; i < all_ui.size(); ++i)
	{
		const SDL_Rect& rect = all_ui[i]->get_rect();

		if(rect.y + rect.h > logical_mouse_y
		&& rect.y < logical_mouse_y
		&& rect.x + rect.w > logical_mouse_x
		&& rect.x < logical_mouse_x)
		{
			return true;
		}
	}
	return false;
}

std::vector<Ui*> Ui::get_navigation_nodes()
{
	return {this};
}