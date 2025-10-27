#include "ui.h"

bool Ui::lock = true;

Ui::Ui()
	:select_on_up(nullptr), select_on_down(nullptr), select_on_left(nullptr), select_on_right(nullptr), 
	state(State::NORMAL), 
	select_sound("sounds/base_select.ogg"), click_sound("sounds/base_click.ogg"), 
	is_selected_sound_played(false),
	position{0, 0, 0, 0},
	callback_function(nullptr)
{

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
	this->is_selected_sound_played = false;
}

void Ui::on_input_pressed(const SDL_Event& e)
{
	if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP || e.key.keysym.sym == SDLK_UP)
	{
		if(this->lock && this->state == State::SELECTED && this->select_on_up != nullptr)
		{
			this->select_new(this->select_on_up);
			this->lock = false;
		}
	}
	else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN || e.key.keysym.sym == SDLK_DOWN)
	{
		if(this->lock && this->state == State::SELECTED && this->select_on_down != nullptr)
		{
			this->select_new(this->select_on_down);
			this->lock = false;
		}
	}
	else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT || e.key.keysym.sym == SDLK_LEFT)
	{
		if(this->lock && this->state == State::SELECTED && this->select_on_left != nullptr)
		{
			this->select_new(this->select_on_left);
			this->lock = false;
		}
	}
	else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT || e.key.keysym.sym == SDLK_RIGHT)
	{
		if(this->lock && this->state == State::SELECTED && this->select_on_right != nullptr)
		{
			this->select_new(this->select_on_right);
			this->lock = false;
		}
	}
	else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A || e.key.keysym.sym == SDLK_RETURN)
	{
		if(this->lock && this->state == State::SELECTED)
		{
			this->on_pointer_down();
			this->lock = false;
		}
	}
}

void Ui::on_input_released(const SDL_Event& e)
{
	if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP || e.key.keysym.sym == SDLK_UP)
	{
		this->lock = true;
		this->is_selected_sound_played = false;
	}
	else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN || e.key.keysym.sym == SDLK_DOWN)
	{
		this->lock = true;
		this->is_selected_sound_played = false;
	}
	else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT || e.key.keysym.sym == SDLK_LEFT)
	{
		this->lock = true;
		this->is_selected_sound_played = false;
	}
	else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT || e.key.keysym.sym == SDLK_RIGHT)
	{
		this->lock = true;
		this->is_selected_sound_played = false;
	}
	else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A || e.key.keysym.sym == SDLK_RETURN)
	{
		if(this->state == State::CLICKED)
		{
			this->on_pointer_up();
			this->lock = true;
			this->is_selected_sound_played = false;
		}
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
			if(e.button.button == SDL_BUTTON_LEFT && this->is_mouse_on_ui()) //TODO : non compatible avec le Slider (this->is_mouse_on_ui())
			{
				this->on_pointer_down();
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if(e.button.button == SDL_BUTTON_LEFT && this->is_mouse_on_ui()) //TODO : non compatible avec le Slider (this->is_mouse_on_ui())
			{
				this->on_pointer_up();
			}
			break;

		case SDL_MOUSEMOTION:
		{
			if(this->is_mouse_on_ui()) //TODO : non compatible avec le Slider (this->is_mouse_on_ui())
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