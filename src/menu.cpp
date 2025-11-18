#include "menu.h"

#include <iostream>

Menu::Menu(std::vector<Ui*> ui, Ui* ui_selected)
	: ui(ui), previous_selected(nullptr), current_selected(ui_selected)
{
	assign_ui_on_moving();
	ui_selected->state = State::SELECTED;
}

bool Menu::is_ui1_facing_ui2(const SDL_Rect pos_ui1, const SDL_Rect pos_ui2, const Axis mode)
{
	if(mode == Axis::X_AXIS)
	{
		return (pos_ui1.x >= pos_ui2.x && pos_ui1.x <= pos_ui2.x + pos_ui2.w)
			|| (pos_ui1.x + pos_ui1.w >= pos_ui2.x && pos_ui1.x + pos_ui1.w <= pos_ui2.x + pos_ui2.w);
	}
	else if(mode == Axis::Y_AXIS)
	{
		return (pos_ui1.y >= pos_ui2.y && pos_ui1.y <= pos_ui2.y + pos_ui2.h)
			|| (pos_ui1.y + pos_ui1.h >= pos_ui2.y && pos_ui1.y + pos_ui1.h <= pos_ui2.y + pos_ui2.h);
	}
	return false; 
}

bool Menu::is_candidate_closer(const Ui* const ui, const Ui* const candidate, const Ui* const current_best, const Axis mode)
{
	const int diff_best_x = std::abs(ui->get_rect().x - current_best->get_rect().x);
	const int diff_candidate_x = std::abs(ui->get_rect().x - candidate->get_rect().x);
	const int diff_best_y = std::abs(ui->get_rect().y - current_best->get_rect().y);
	const int diff_candidate_y = std::abs(ui->get_rect().y - candidate->get_rect().y);

	if(mode == Axis::X_AXIS)
	{
		return diff_best_x > diff_candidate_x || (diff_best_x == diff_candidate_x && diff_best_y > diff_candidate_y);
	}
	else if(mode == Axis::Y_AXIS)
	{
		return diff_best_y > diff_candidate_y || (diff_best_y == diff_candidate_y && diff_best_x > diff_candidate_x);
	}
	return false;
}

//ui = ui to which we assign a "select_on"
//candidate = the current ui in the inner for loop
//current_best = the "select_on_*" we try to assign to ui (up, down, left or right)
void Menu::get_ui_facing(Ui* ui, Ui* candidate, Ui*& current_best, const Axis mode)
{
	if(current_best == nullptr)
	{
		current_best = candidate;
	}
	else
	{
		const bool ui_facing_current_best = is_ui1_facing_ui2(ui->get_rect(), current_best->get_rect(), mode);
		const bool ui_facing_candidate = is_ui1_facing_ui2(ui->get_rect(), candidate->get_rect(), mode);
		const bool ui_facing_both = ui_facing_current_best && ui_facing_candidate;
		const bool ui_facing_neither = !ui_facing_current_best && !ui_facing_candidate;

		//if the two ui are facing each other ou if neither of them are facing each other, take the one with the lowest diff* 
		//if current_best is not facing ui but candidate is facing ui, take candidate
		if(ui_facing_both || ui_facing_neither)
		{
			if(is_candidate_closer(ui, candidate, current_best, mode))
			{
				current_best = candidate;
			}
		}
		else if(!ui_facing_current_best && ui_facing_candidate)
		{
			current_best = candidate;
		}
	}
}

void Menu::assign_ui_on_moving()
{
	std::vector<Ui*> navigation_list;
	for(Ui* ui : this->ui)
	{
		std::vector<Ui*> nodes = ui->get_navigation_nodes();
		navigation_list.insert(navigation_list.end(), nodes.begin(), nodes.end());
	}

	for(Ui* ui : navigation_list)
	{
		Ui* current_best_up = nullptr;
		Ui* current_best_down = nullptr;
		Ui* current_best_left = nullptr;
		Ui* current_best_right = nullptr;

		for(Ui* candidate : navigation_list)
		{
			if(candidate == ui) { continue; }

			const SDL_Rect candidate_pos = candidate->get_rect();
			const SDL_Rect ui_pos = ui->get_rect();

			if(candidate_pos.y + candidate_pos.h <= ui_pos.y)
			{
				get_ui_facing(ui, candidate, current_best_up, Axis::X_AXIS);
			}
			if(candidate_pos.y >= ui_pos.y + ui_pos.h)
			{
				get_ui_facing(ui, candidate, current_best_down, Axis::X_AXIS);
			}
			if(candidate_pos.x + candidate_pos.w <= ui_pos.x)
			{
				get_ui_facing(ui, candidate, current_best_left, Axis::Y_AXIS);
			}
			if(candidate_pos.x >= ui_pos.x + ui_pos.w)
			{
				get_ui_facing(ui, candidate, current_best_right, Axis::Y_AXIS);
			}
		}
		ui->select_on_up = current_best_up;
		ui->select_on_down = current_best_down;
		ui->select_on_left = current_best_left;
		ui->select_on_right = current_best_right;
	}
}

Menu::~Menu()
{
	for(Ui* ui : this->ui)
	{
		delete ui; 
	}
}

void Menu::handle_events(const SDL_Event& e)
{
	for(Ui* ui : this->ui)
	{
		if(e.type == SDL_MOUSEMOTION)
		{
			if(ui->is_mouse_on_ui() != ui->MOUSE_NOT_ON_ANY_UI) 
			{
				if(dynamic_cast<ToggleGroup*>(ui) != nullptr)
				{
					//ui est le toggle dont l'indice a été retourné par is_mouse_on_ui()
					ToggleGroup* togglegroup = dynamic_cast<ToggleGroup*>(ui);
					ui = togglegroup->toggles[ui->is_mouse_on_ui()];
				}

				if(dynamic_cast<TextToggleGroup*>(ui) != nullptr)
				{
					//ui est le toggle dont l'indice a été retourné par is_mouse_on_ui()
					TextToggleGroup* texttogglegroup = dynamic_cast<TextToggleGroup*>(ui);
					ui = texttogglegroup->toggles[ui->is_mouse_on_ui()].get();
				}

				if(this->previous_selected != nullptr && ui != this->previous_selected && this->previous_selected->state != State::NORMAL) 
				{
					previous_selected->state = State::NORMAL; 
					if(dynamic_cast<Slider*>(previous_selected) != nullptr)
					{
						dynamic_cast<Slider*>(previous_selected)->is_selected = false;
						dynamic_cast<Slider*>(previous_selected)->is_dragged = false;
					}
				}
				this->previous_selected = this->current_selected;
				this->current_selected = ui;
			}
		}
		ui->handle_events(e);
	}
}

void Menu::draw(SDL_Renderer* renderer)
{
	for(Ui* ui : this->ui)
	{
		ui->draw(renderer);
	}
}

void Menu::update(Uint64 time_step)
{
	for(Ui* ui : this->ui)
	{
		ui->update(time_step);
		if(ui->state == State::SELECTED)
		{
			this->previous_selected = this->current_selected;
			this->current_selected = ui;
		}
	}
}