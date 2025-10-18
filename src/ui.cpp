#include "ui.h"

#include <iostream>

Ui::Ui()
	:select_on_up(nullptr), select_on_down(nullptr), select_on_left(nullptr), select_on_right(nullptr), state(State::NORMAL), select_sound("sounds/base_select.ogg"), click_sound("sounds/base_click.ogg")
{
	std::cout << "Constructor from base class" << std::endl;
}

Ui::~Ui()
{
	std::cout << "Destructor from base class" << std::endl;
}

bool Ui::is_mouse_on_ui()
{
	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	return (this->position.y + this->position.h > mouse_y
		 && this->position.y < mouse_y
		 && this->position.x + this->position.w > mouse_x
		 && this->position.x < mouse_x);
}

void Ui::handle_events(SDL_Event e)
{
	switch(e.type)
	{
		case SDL_KEYDOWN:
			this->on_key_pressed();
			break;

		case SDL_KEYUP:
			this->on_key_released();
			break;

		case SDL_MOUSEBUTTONDOWN:
			if(this->is_mouse_on_ui())
			{
				this->on_pointer_down();
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if(this->is_mouse_on_ui())
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
	}
}