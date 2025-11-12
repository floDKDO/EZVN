#include "ui.h"

bool Ui::lock = true;

Ui::Ui()
	:select_on_up(nullptr), select_on_down(nullptr), select_on_left(nullptr), select_on_right(nullptr), 
	state(State::NORMAL), 
	select_sound("sounds/base_select.ogg"), click_sound("sounds/base_click.ogg"), 
	is_selected_sound_played(false),
	position{0, 0, 0, 0},
	callback_function(nullptr),
	pointer_on_ui_when_pointer_up(true)
{

}

#include <iostream>
Ui::~Ui()
{
	std::cout << "UI" << std::endl;
}

void Ui::select_new(Ui* ui)
{
	this->state = State::NORMAL;
	ui->state = State::SELECTED;
	if(!this->is_selected_sound_played)
	{
		this->select_sound.play_sound();
		this->is_selected_sound_played = true;
	}
}

void Ui::on_pointer_up() //TODO : mettre les if dans handle_events ??
{
	if(this->pointer_on_ui_when_pointer_up)
	{
		if(this->state == State::CLICKED)
		{
			this->state = State::SELECTED;
			callback_function(this);
			this->click_sound.play_sound();
		}
	}
	else //the callback function is called even if the pointer is not on the UI when the pointer is released/up
	{
		callback_function(this);
		this->click_sound.play_sound();
	}
}

void Ui::on_pointer_down() 
{
	this->state = State::CLICKED;
}

void Ui::on_pointer_enter() 
{
	if(this->state == State::NORMAL) 
	{
		this->state = State::SELECTED;
		if(!this->is_selected_sound_played)
		{
			this->select_sound.play_sound();
			this->is_selected_sound_played = true;
		}
	}
}

void Ui::on_pointer_exit() 
{
	if(this->state == State::CLICKED)
	{
		this->state = State::SELECTED; 
	}
	this->is_selected_sound_played = false;
}

void Ui::on_up_pressed()
{
	if(this->lock && this->state == State::SELECTED && this->select_on_up != nullptr)
	{
		this->select_new(this->select_on_up);
		this->lock = false;
	}
}

void Ui::on_down_pressed()
{
	if(this->lock && this->state == State::SELECTED && this->select_on_down != nullptr)
	{
		this->select_new(this->select_on_down);
		this->lock = false;
	}
}

void Ui::on_left_pressed()
{
	if(this->lock && this->state == State::SELECTED && this->select_on_left != nullptr)
	{
		this->select_new(this->select_on_left);
		this->lock = false;
	}
}

void Ui::on_right_pressed()
{
	if(this->lock && this->state == State::SELECTED && this->select_on_right != nullptr)
	{
		this->select_new(this->select_on_right);
		this->lock = false;
	}
}

void Ui::on_enter_pressed()
{
	if(this->lock && this->state == State::SELECTED) //TODO : remettre les if des fonctions on_***_pressed dans on_input_pressed ??
	{
		this->state = State::CLICKED;
		this->lock = false;
	}
}

void Ui::on_input_pressed(const SDL_Event& e)
{
	if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP || e.key.keysym.sym == SDLK_UP)
	{
		this->on_up_pressed();
	}
	else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN || e.key.keysym.sym == SDLK_DOWN)
	{
		this->on_down_pressed();
	}
	else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT || e.key.keysym.sym == SDLK_LEFT)
	{
		this->on_left_pressed();
	}
	else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT || e.key.keysym.sym == SDLK_RIGHT)
	{
		this->on_right_pressed();
	}
	else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A || e.key.keysym.sym == SDLK_RETURN)
	{
		this->on_enter_pressed();
	}
}

void Ui::on_up_released()
{
	this->lock = true;
	this->is_selected_sound_played = false;
}

void Ui::on_down_released()
{
	this->lock = true;
	this->is_selected_sound_played = false;
}

void Ui::on_left_released()
{
	this->lock = true;
	this->is_selected_sound_played = false;
}

void Ui::on_right_released()
{
	this->lock = true;
	this->is_selected_sound_played = false;
}

void Ui::on_enter_released()
{
	if(this->state == State::CLICKED)
	{
		this->state = State::SELECTED;
		callback_function(this);
		this->click_sound.play_sound();

		this->lock = true;
		this->is_selected_sound_played = false; //TODO : inutile ??
	}
}

void Ui::on_input_released(const SDL_Event& e)
{
	if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP || e.key.keysym.sym == SDLK_UP)
	{
		this->on_up_released();
	}
	else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN || e.key.keysym.sym == SDLK_DOWN)
	{
		this->on_down_released();
	}
	else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT || e.key.keysym.sym == SDLK_LEFT)
	{
		this->on_left_released();
	}
	else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT || e.key.keysym.sym == SDLK_RIGHT)
	{
		this->on_right_released();
	}
	else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A || e.key.keysym.sym == SDLK_RETURN)
	{
		this->on_enter_released();
	}
}

void Ui::handle_events(const SDL_Event& e)
{
	switch(e.type)
	{
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_KEYDOWN:
			this->on_input_pressed(e);
			break;

		case SDL_CONTROLLERBUTTONUP:
		case SDL_KEYUP:
			this->on_input_released(e);
			break;

		case SDL_MOUSEBUTTONDOWN:
			if(e.button.button == SDL_BUTTON_LEFT && this->is_mouse_on_ui()) 
			{
				this->on_pointer_down();
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if(e.button.button == SDL_BUTTON_LEFT && (this->state == State::SELECTED || this->state == State::CLICKED)) //cas "this->state == State::SELECTED" uniquement pour pouvoir bouger la poignée du Slider sans que la souris soit sur la poignée
			{
				this->on_pointer_up();
			}
			break;

		case SDL_MOUSEMOTION:
		{
			if(this->is_mouse_on_ui())
			{
				this->on_pointer_enter();
			}
			else
			{
				this->on_pointer_exit();
			}
		}
		break;

		default:
			break;
	}
}

bool Ui::is_mouse_on_ui() const
{
	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	return (this->position.y + this->position.h > mouse_y
		 && this->position.y < mouse_y
		 && this->position.x + this->position.w > mouse_x
		 && this->position.x < mouse_x);
}