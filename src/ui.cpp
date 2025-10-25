#include "ui.h"

#include <iostream>

bool Ui::lock = true;

Ui::Ui()
	:select_on_up(nullptr), select_on_down(nullptr), select_on_left(nullptr), select_on_right(nullptr), 
	state(State::NORMAL), 
	select_sound("sounds/base_select.ogg"), click_sound("sounds/base_click.ogg"), 
	is_selected_sound_played(false),
	position{0, 0, 0, 0},
	callback_function(nullptr)
{
	std::cout << "CALLED (base)!\n";
}

Ui::~Ui()
{
	
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

void Ui::on_pointer_up()
{
	this->state = State::SELECTED;
	callback_function(this);
	this->click_sound.play_sound();
}

void Ui::on_pointer_down() 
{
	this->state = State::CLICKED;
}

void Ui::on_pointer_enter() 
{
	this->state = State::SELECTED;
	if(!this->is_selected_sound_played)
	{
		this->select_sound.play_sound();
		this->is_selected_sound_played = true;
	}
}

void Ui::on_pointer_exit() 
{
	//this->state = State::NORMAL;
	this->is_selected_sound_played = false;
}

void Ui::on_key_pressed(const SDL_Event& e)
{
	switch(e.key.keysym.sym)
	{
		case SDLK_UP:
			if(this->lock && this->state == State::SELECTED && this->select_on_up != nullptr)
			{
				//std::cout << "LA, " << this->name << "\n";
				this->select_new(this->select_on_up);
				this->lock = false;
			}
			break;

		case SDLK_DOWN:
			if(this->lock && this->state == State::SELECTED && this->select_on_down != nullptr)
			{
				//std::cout << "ICI, " << this->name << "\n";
				this->select_new(this->select_on_down);
				this->lock = false;
			}
			break;

		case SDLK_LEFT:
			if(this->lock && this->state == State::SELECTED && this->select_on_left != nullptr)
			{
				this->select_new(this->select_on_left);
				this->lock = false;
			}
			break;

		case SDLK_RIGHT:
			//std::cout << this->name << ", " << this->lock << ", " << int(this->state) << ", " << this->is_selected_sound_played << std::endl;
			if(this->lock && this->state == State::SELECTED && this->select_on_right != nullptr)
			{
				this->select_new(this->select_on_right);
				this->lock = false;
			}
			
			break;

		case SDLK_RETURN:
			if(this->state == State::SELECTED)
			{
				this->on_pointer_down();
				this->lock = false;
			}
			break;

		default:
			break;
	}
	
}

void Ui::on_key_released(const SDL_Event& e) 
{
	switch(e.key.keysym.sym)
	{
		case SDLK_UP:
			this->lock = true;
			this->is_selected_sound_played = false;
			break;

		case SDLK_DOWN:
			this->lock = true;
			this->is_selected_sound_played = false;
			break;

		case SDLK_LEFT:
			this->lock = true;
			this->is_selected_sound_played = false;
			break;

		case SDLK_RIGHT:
			this->lock = true;
			this->is_selected_sound_played = false;
			break;

		case SDLK_RETURN:
			if(this->state == State::CLICKED)
			{
				this->on_pointer_up();
				this->lock = true;
				this->is_selected_sound_played = false;
			}
			break;

		default:
			break;
	}
	
}

void Ui::on_button_pressed(const SDL_Event& e)
{
	//if(this->lock)
	{
		switch(e.cbutton.button)
		{
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				if(this->state == State::SELECTED && this->select_on_up != nullptr)
				{
					this->select_new(this->select_on_up);
				}
				break;

			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				if(this->state == State::SELECTED && this->select_on_down != nullptr)
				{
					this->select_new(this->select_on_down);
				}
				break;

			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				if(this->state == State::SELECTED && this->select_on_left != nullptr)
				{
					this->select_new(this->select_on_left);
				}
				break;

			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				if(this->state == State::SELECTED && this->select_on_right != nullptr)
				{
					this->select_new(this->select_on_right);
				}
				break;

			case SDL_CONTROLLER_BUTTON_A:
				if(this->state == State::SELECTED)
				{
					this->on_pointer_down();
				}
				break;

			default:
				break;
		}
	}
	//this->lock = false;
}

void Ui::on_button_released(const SDL_Event& e)
{
	switch(e.cbutton.button)
	{
		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			break;

		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			break;

		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			break;

		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			break;

		case SDL_CONTROLLER_BUTTON_A:
			this->on_pointer_up();
			break;

		default:
			break;
	}
	//this->lock = true;
}

void Ui::handle_events(const SDL_Event& e)
{
	switch(e.type)
	{
		case SDL_KEYDOWN:
			this->on_key_pressed(e);
			break;

		case SDL_KEYUP:
			this->on_key_released(e);
			break;

		case SDL_CONTROLLERBUTTONDOWN:
			this->on_button_pressed(e);
			break;

		case SDL_CONTROLLERBUTTONUP:
			this->on_button_released(e);
			break;

		case SDL_MOUSEBUTTONDOWN:
			if(e.button.button == SDL_BUTTON_LEFT && this->is_mouse_on_ui())
			{
				this->on_pointer_down();
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if(e.button.button == SDL_BUTTON_LEFT && this->is_mouse_on_ui())
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
			else this->on_pointer_exit();
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