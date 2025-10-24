#include "menu.h"

#include <iostream>

Menu::Menu(std::vector<Ui*>& ui)
	: ui(ui), previous_selected(nullptr), current_selected(nullptr)
{
	assign_ui_on_moving();
}

bool Menu::is_ui_facing(Ui* ui1, Ui* ui2, char mode)
{
	if(mode == 'x')
	{
		return (ui1->position.x >= ui2->position.x && ui1->position.x <= ui2->position.x + ui2->position.w)
			|| (ui1->position.x + ui1->position.w >= ui2->position.x && ui1->position.x + ui1->position.w <= ui2->position.x + ui2->position.w);
	}
	else if(mode == 'y')
	{
		return (ui1->position.y >= ui2->position.y && ui1->position.y <= ui2->position.y + ui2->position.h)
			|| (ui1->position.y + ui1->position.h >= ui2->position.y && ui1->position.y + ui1->position.h <= ui2->position.y + ui2->position.h);
	}
	else
	{
		return false;
	}
}

void Menu::assign_ui_on_moving()
{
	for(int i = 0; i < ui.size(); ++i)
	{
		Ui* current_ui = ui[i];

		Ui* temp_select_on_up = nullptr;
		Ui* temp_select_on_down = nullptr;
		Ui* temp_select_on_left = nullptr;
		Ui* temp_select_on_right = nullptr;

		for(int j = 0; j < ui.size(); ++j)
		{
			if(ui[j] == current_ui)
			{
				continue;
			}

			if(ui[j]->position.y + ui[j]->position.h <= current_ui->position.y
			&& is_ui_facing(ui[j], current_ui, 'x'))
			{
				if(temp_select_on_up == nullptr)
				{
					temp_select_on_up = ui[j];
				}
				else
				{
					int diff1_y = current_ui->position.y - temp_select_on_up->position.y;
					int diff2_y = current_ui->position.y - ui[j]->position.y;

					if(diff1_y > diff2_y)
					{
						temp_select_on_up = ui[j];
					}
				}
			}
			else if(ui[j]->position.y >= current_ui->position.y + current_ui->position.h
			&& is_ui_facing(ui[j], current_ui, 'x'))
			{
				if(temp_select_on_down == nullptr)
				{
					temp_select_on_down = ui[j];
				}
				else
				{
					int diff1_y = temp_select_on_down->position.y - current_ui->position.y;
					int diff2_y = ui[j]->position.y - current_ui->position.y;

					if(diff1_y > diff2_y)
					{
						temp_select_on_down = ui[j];
					}
				}
			}
			else if(ui[j]->position.x + ui[j]->position.w <= current_ui->position.x
			&& is_ui_facing(ui[j], current_ui, 'y'))
			{
				if(temp_select_on_left == nullptr)
				{
					temp_select_on_left = ui[j];
				}
				else
				{
					int diff1_x = current_ui->position.x - temp_select_on_left->position.x;
					int diff2_x = current_ui->position.x - ui[j]->position.x;

					if(diff1_x > diff2_x)
					{
						temp_select_on_left = ui[j];
					}
				}
			}
			else if(ui[j]->position.x >= current_ui->position.x + current_ui->position.w
			&& is_ui_facing(ui[j], current_ui, 'y'))
			{
				if(temp_select_on_right == nullptr)
				{
					temp_select_on_right = ui[j];
				}
				else
				{
					int diff1_x = temp_select_on_right->position.x - current_ui->position.x;
					int diff2_x = ui[j]->position.x - current_ui->position.x;

					if(diff1_x > diff2_x)
					{
						temp_select_on_right = ui[j];
					}
				}
			}
		}
		current_ui->select_on_up = temp_select_on_up;
		current_ui->select_on_down = temp_select_on_down;
		current_ui->select_on_left = temp_select_on_left;
		current_ui->select_on_right = temp_select_on_right;
	}
}


Menu::~Menu()
{
	for(Ui*& ui : this->ui)
	{
		std::cout << "DESTRUCTEUR !";
		delete ui;
	}
}

void Menu::handle_events(const SDL_Event& e)
{
	for(Ui*& ui : this->ui)
	{
		if(e.type == SDL_MOUSEMOTION)
		{
			if(ui->is_mouse_on_ui())
			{
				if(this->previous_selected != nullptr)
					previous_selected->state = State::NORMAL;
				this->previous_selected = this->current_selected;
				this->current_selected = ui;
			}
		}
		ui->handle_events(e);
	}
}

void Menu::draw(SDL_Renderer* renderer)
{
	for(Ui*& ui : this->ui)
	{
		ui->draw(renderer);
	}
}

void Menu::update(Uint64& time_step)
{
	for(Ui*& ui : this->ui)
	{
		ui->update(time_step);
		if(ui->state == State::SELECTED)
		{
			this->previous_selected = this->current_selected;
			this->current_selected = ui;
		}
		//std::cout << ui->name << " : " << int(ui->state) << std::endl;
	}
}


//void Menu::assign_ui_on_moving()
//{
//	for(int i = 0; i < ui.size(); ++i)
//	{
//		Ui* current_ui = ui[i];
//
//		/*bool chose_select_on_up = false;
//		bool chose_select_on_down = false;
//		bool chose_select_on_left = false;
//		bool chose_select_on_right = false;*/
//
//		Ui* temp_select_on_up = nullptr;
//		Ui* temp_select_on_down = nullptr;
//		Ui* temp_select_on_left = nullptr;
//		Ui* temp_select_on_right = nullptr;
//
//		for(int j = 0; j < ui.size(); ++j)
//		{
//			if(ui[j] == current_ui)
//			{
//				continue;
//			}
//
//			/*if(chose_select_on_up && chose_select_on_down && chose_select_on_left && chose_select_on_right)
//			{
//				break;
//			}*/
//
//			if(/*!chose_select_on_up &&*/ ui[j]->position.y + ui[j]->position.h <= current_ui->position.y
//			&& is_ui_facing(ui[j], current_ui, 'x'))
//			{
//				//current_ui->select_on_up = ui[j];
//				//chose_select_on_up = true;
//
//				if(temp_select_on_up == nullptr)
//				{
//					temp_select_on_up = ui[j];
//				}
//				else
//				{
//					int diff1_y = current_ui->position.y - temp_select_on_up->position.y;
//					int diff2_y = current_ui->position.y - ui[j]->position.y;
//
//					if(diff1_y > diff2_y)
//					{
//						temp_select_on_up = ui[j];
//					}
//				}
//			}
//			if(/*!chose_select_on_down &&*/ ui[j]->position.y >= current_ui->position.y + current_ui->position.h
//			&& is_ui_facing(ui[j], current_ui, 'x'))
//			{
//				//current_ui->select_on_down = ui[j];
//				//chose_select_on_down = true;
//
//				if(temp_select_on_down == nullptr)
//				{
//					temp_select_on_down = ui[j];
//				}
//				else
//				{
//					int diff1_y = temp_select_on_down->position.y - current_ui->position.y;
//					int diff2_y = ui[j]->position.y - current_ui->position.y;
//
//					if(diff1_y > diff2_y)
//					{
//						temp_select_on_down = ui[j];
//					}
//				}
//			}
//			if(/*!chose_select_on_left &&*/ ui[j]->position.x + ui[j]->position.w <= current_ui->position.x
//			&& is_ui_facing(ui[j], current_ui, 'y'))
//			{
//				//current_ui->select_on_left = ui[j];
//				//chose_select_on_left = true;
//
//				if(temp_select_on_left == nullptr)
//				{
//					temp_select_on_left = ui[j];
//				}
//				else
//				{
//					int diff1_x = current_ui->position.x - temp_select_on_left->position.x;
//					int diff2_x = current_ui->position.x - ui[j]->position.x;
//
//					if(diff1_x > diff2_x)
//					{
//						temp_select_on_left = ui[j];
//					}
//				}
//			}
//			if(/*!chose_select_on_right &&*/ ui[j]->position.x >= current_ui->position.x + current_ui->position.w
//			&& is_ui_facing(ui[j], current_ui, 'y'))
//			{
//				//current_ui->select_on_right = ui[j];
//				//chose_select_on_right = true;
//
//				if(temp_select_on_right == nullptr)
//				{
//					temp_select_on_right = ui[j];
//				}
//				else
//				{
//					int diff1_x = temp_select_on_right->position.x - current_ui->position.x;
//					int diff2_x = ui[j]->position.x - current_ui->position.x;
//
//					if(diff1_x > diff2_x)
//					{
//						temp_select_on_right = ui[j];
//					}
//				}
//			}
//		}
//		current_ui->select_on_up = temp_select_on_up;
//		current_ui->select_on_down = temp_select_on_down;
//		current_ui->select_on_left = temp_select_on_left;
//		current_ui->select_on_right = temp_select_on_right;
//	}
//}