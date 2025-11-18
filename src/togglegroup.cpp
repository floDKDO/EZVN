#include "togglegroup.h"

#include <iostream>

//TODO : bien buggué...
//TODO : pourquoi pas renommer cette classe en UIContainer qui contient un vector de Ui*

ToggleGroup::ToggleGroup(size_t number, std::string text, const std::string path_normal, const std::string path_selected, const std::string path_clicked, const std::string path_checked, const int x, const int y, bool is_checked, SDL_Renderer* renderer, std::vector<std::function<void(Ui* ui)>> callback_functions)
	: number(number), text(text, {255, 255, 255, 255}, "fonts/Aller_Rg.ttf", 50, x, y - 100, renderer), only_one_has_to_be_checked(true) //TODO : paramètre du constructeur
{
	SDL_assert(callback_functions.size() == number);

	this->renderer = renderer;

	for(unsigned int i = 0; i < number; ++i)
	{
		if(i == 0)
			is_checked = true;
		else is_checked = false;

		Toggle* toggle_i = new Toggle(path_normal, path_selected, path_clicked, path_checked, x, y + 200 * i, is_checked, renderer, callback_functions[i]);
		this->toggles.push_back(toggle_i); 
	}
}

void ToggleGroup::uncheck_all_others(unsigned int index_to_not_uncheck)
{
	for(int i = 0; i < toggles.size(); ++i)
	{
		Toggle* t = toggles[i];
		if(i != index_to_not_uncheck)
		{
			t->is_checked = false;
		}
	}
}

void ToggleGroup::on_pointer_up()
{
	for(int i = 0; i < toggles.size(); ++i)
	{
		Toggle* t = toggles[i];

		if(t->state == State::CLICKED)
		{
			t->state = State::SELECTED;
			t->callback_function(t);
			t->click_sound.play_sound();

			//TODO
			if(only_one_has_to_be_checked)
			{
				if(!t->is_checked)
				{
					t->is_checked = true;
				}
				this->uncheck_all_others(i);
				break;
			}
			else
			{
				t->on_pointer_up_hook_end();
			}
			////////////////////////////////////////////////
		}
	}
}

void ToggleGroup::on_enter_released() //TODO : pas ouf car répétition avec Ui
{
	std::cout << "Appel : " << toggles.size() << std::endl;

	int index = -1;
	for(int i = 0; i < toggles.size(); ++i)
	{
		Toggle* t = toggles[i];
		std::cout << "STATE : " << i << " -> " << int(t->state) << std::endl;
		if(t->state == State::CLICKED)
		{
			std::cout << "DEDANS\n";
			t->state = State::SELECTED;
			t->callback_function(t);
			t->click_sound.play_sound();
			if(only_one_has_to_be_checked)
			{
				if(!t->is_checked)
					t->is_checked = true;

				index = i;
			}
			else
			{
				t->is_checked = !t->is_checked;
			}
		}
		t->lock = true;
	}

	std::cout << "INDEX : " << index << std::endl;


	if(index != -1 && only_one_has_to_be_checked)
	{
		for(int i = 0; i < toggles.size(); ++i)
		{
			Toggle* t = toggles[i];
			if(t->state == State::NORMAL)
			{
				if(i != index)
				{
					t->is_checked = false;
				}
			}
		}
	}
}

void ToggleGroup::on_input_released(const SDL_Event& e)
{
	for(Toggle* t : toggles)
	{
		if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP || e.key.keysym.sym == SDLK_UP)
		{
			t->on_up_released();
		}
		else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN || e.key.keysym.sym == SDLK_DOWN)
		{
			t->on_down_released();
		}
		else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT || e.key.keysym.sym == SDLK_LEFT)
		{
			t->on_left_released();
		}
		else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT || e.key.keysym.sym == SDLK_RIGHT)
		{
			t->on_right_released();
		}
		else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A || e.key.keysym.sym == SDLK_RETURN)
		{
			this->on_enter_released();
		}
	}
}


void ToggleGroup::draw(SDL_Renderer* renderer)
{
	for(Toggle* t : toggles)
	{
		t->draw(renderer);
	}
	this->text.draw(renderer);
}

void ToggleGroup::update(Uint64 time_step)
{
	for(Toggle* t : toggles)
	{
		t->update(time_step);
	}
	this->text.update(time_step);
}

void ToggleGroup::handle_events(const SDL_Event& e) 
{
	for(Toggle* t : toggles)
	{
		switch(e.type)
		{
			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_KEYDOWN:
				t->on_input_pressed(e);
				break;

			case SDL_CONTROLLERBUTTONUP:
			case SDL_KEYUP:
				this->on_input_released(e); //TODO : seule différence
				break;

			case SDL_MOUSEBUTTONDOWN:
				if(e.button.button == SDL_BUTTON_LEFT && t->is_mouse_on_ui() != MOUSE_NOT_ON_ANY_UI)
				{
					t->on_pointer_down();
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if(e.button.button == SDL_BUTTON_LEFT && (t->state == State::SELECTED || t->state == State::CLICKED)) //cas "this->state == State::SELECTED" uniquement pour pouvoir bouger la poignée du Slider sans que la souris soit sur la poignée
				{
					this->on_pointer_up(); //TODO : seule différence
				}
				break;

			case SDL_MOUSEMOTION:
			{
				if(t->is_mouse_on_ui() != MOUSE_NOT_ON_ANY_UI)
				{
					t->on_pointer_enter();
				}
				else
				{
					t->on_pointer_exit();
				}
			}
			break;

			default:
				break;
		}
	}
}

std::vector<Ui*> ToggleGroup::get_navigation_nodes()
{
	std::vector<Ui*> vector;
	for(Ui* ui : toggles)
	{
		vector.push_back(ui);
	}
	return vector;
}

SDL_Rect ToggleGroup::get_rect() const //TODO : INUTILE !!
{
	return {0, 0, 0, 0};
}