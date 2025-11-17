#include "texttogglegroup.h"

#include <iostream>

//TODO : bien buggué...
//TODO : pourquoi pas renommer cette classe en UIContainer qui contient un vector de Ui*

//TODO : vector de callback functions
TextToggleGroup::TextToggleGroup(size_t number, std::string top_text, std::vector<std::string>texts, SDL_Color color_unchecked, SDL_Color color_selected, SDL_Color color_checked, const int x, const int y, bool is_checked, SDL_Renderer* renderer, std::vector<std::function<void(Ui* ui)>> callback_functions)
	: number(number), top_text(top_text, {255, 255, 255, 255}, "fonts/Aller_Rg.ttf", 50, x, y - 100, renderer), only_one_has_to_be_checked(true) //TODO : paramètre du constructeur
{
	SDL_assert(texts.size() == number && callback_functions.size() == number);

	//this->callback_function = callback_function; //TODO : inutile ??
	this->pointer_on_ui_when_pointer_up = true;
	this->renderer = renderer;

	for(unsigned int i = 0; i < number; ++i)
	{
		if(i == 0)
			is_checked = true;
		else is_checked = false;

		TextToggle* toggle_i = new TextToggle(texts[i], color_unchecked, color_selected, color_checked, x, y + 100 * i, is_checked, renderer, callback_functions[i]);
		this->toggles.push_back(toggle_i); 
	}
}

void TextToggleGroup::on_pointer_up()
{
	std::cout << "Appel : " << toggles.size() << std::endl;

	int index = -1;
	for(int i = 0; i < toggles.size(); ++i)
	{
		TextToggle* t = toggles[i];
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
	}

	std::cout << "INDEX : " << index << std::endl;

	if(index != -1 && only_one_has_to_be_checked)
	{
		for(int i = 0; i < toggles.size(); ++i)
		{
			TextToggle* t = toggles[i];
			if(i != index)
			{
				t->is_checked = false;
			}
		}
	}
}

void TextToggleGroup::on_enter_released() //TODO : pas ouf car répétition avec Ui
{
	std::cout << "Appel : " << toggles.size() << std::endl;

	int index = -1;
	for(int i = 0; i < toggles.size(); ++i)
	{
		TextToggle* t = toggles[i];
		std::cout << "STATE : " << t->text.text << " -> " << int(t->state) << std::endl;
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
			TextToggle* t = toggles[i];
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

void TextToggleGroup::on_input_released(const SDL_Event& e)
{
	int i = 0;
	for(TextToggle* t : toggles)
	{
		i += 1;
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
		else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A || e.key.keysym.sym == SDLK_RETURN) //TODO important : appelé même si le texttoggle n'est pas sélectionné
		{
			std::cout << "DANS LE SWITCH RELEASED\n";
			this->on_enter_released();
		}
	}

	std::cout << i << std::endl;
}


void TextToggleGroup::draw(SDL_Renderer* renderer)
{
	for(TextToggle* t : toggles)
	{
		t->draw(renderer);
	}
	this->top_text.draw(renderer);
}

void TextToggleGroup::update(Uint64 time_step)
{
	for(TextToggle* t : toggles)
	{
		t->update(time_step);
	}
	this->top_text.update(time_step);
}

void TextToggleGroup::handle_events(const SDL_Event& e)
{
	for(TextToggle* t : toggles)
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

std::vector<Ui*> TextToggleGroup::get_navigation_nodes()
{
	std::vector<Ui*> vector;
	for(Ui* ui : toggles)
	{
		vector.push_back(ui);
	}
	return vector;
}

SDL_Rect TextToggleGroup::get_rect() const //TODO : INUTILE !!
{
	return {0, 0, 0, 0};
}