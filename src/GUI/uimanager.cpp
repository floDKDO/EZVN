#include "GUI/uimanager.h"
#include "GUI/slider.h"
#include "GUI/confirmationpopup.h"

#include <iostream>

UiManager::UiManager(std::vector<std::unique_ptr<Ui>> ui_elements)
	: ui_elements_(std::move(ui_elements)), previous_selected_(nullptr)
{
	for(std::unique_ptr<Ui> const& ui_element : ui_elements_)
	{
		//TODO : dégeulasse !!
		if(dynamic_cast<TextButton*>(ui_element.get()) != nullptr) 
		{
			TextButton* textbutton = dynamic_cast<TextButton*>(ui_element.get());
			if(textbutton->confirmationpopup_ != nullptr)
			{
				std::vector<Ui*> nodes = textbutton->get_navigation_nodes();
				navigation_list_.insert(navigation_list_.end(), nodes.begin(), nodes.end());
				std::vector<Ui*> nodes2 = textbutton->confirmationpopup_->get_navigation_nodes();
				navigation_list_.insert(navigation_list_.end(), nodes2.begin(), nodes2.end());
			}
		}

		std::vector<Ui*> nodes = ui_element->get_navigation_nodes();
		navigation_list_.insert(navigation_list_.end(), nodes.begin(), nodes.end());
	}
	current_selected_ = ui_elements_[0].get();
	current_selected_->state_ = State::SELECTED;

	assign_ui_on_moving();
}

bool UiManager::is_ui1_facing_ui2(const SDL_Rect pos_ui1, const SDL_Rect pos_ui2, const Axis mode) const
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

bool UiManager::is_candidate_closer(const Ui* const ui, const Ui* const candidate, const Ui* const current_best, const Axis mode) const
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

//ui = ui to which we assign a "select_on_*"
//candidate = the current ui in the inner for loop
//current_best = the "select_on_*" we try to assign to ui (up, down, left or right)
void UiManager::get_ui_facing(Ui* ui, Ui* candidate, Ui*& current_best, const Axis mode) const
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

void UiManager::assign_ui_on_moving() const
{
	for(Ui* ui : navigation_list_)
	{
		Ui* current_best_up = nullptr;
		Ui* current_best_down = nullptr;
		Ui* current_best_left = nullptr;
		Ui* current_best_right = nullptr;

		for(Ui* candidate : navigation_list_)
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
		ui->select_on_up_ = current_best_up;
		ui->select_on_down_ = current_best_down;
		ui->select_on_left_ = current_best_left;
		ui->select_on_right_ = current_best_right;
	}
}

void UiManager::handle_events(const SDL_Event& e)
{
	for(Ui* ui : navigation_list_)
	{
		if(Ui::is_pop_up_visible_) //TODO : approximatif
		{
			TextButton* textbutton_confirmationpopup = dynamic_cast<TextButton*>(ui);
			if(textbutton_confirmationpopup->confirmationpopup_ != nullptr)
			{
				textbutton_confirmationpopup->confirmationpopup_->yes_.handle_events(e);
				textbutton_confirmationpopup->confirmationpopup_->no_.handle_events(e);
			}
		}
		else
		{
			if(e.type == SDL_MOUSEMOTION)
			{
				if(ui->is_mouse_on_ui() != ui->MOUSE_NOT_ON_ANY_UI_)
				{
					if(previous_selected_ != nullptr && ui != previous_selected_ && previous_selected_->state_ != State::NORMAL)
					{
						previous_selected_->state_ = State::NORMAL;
						if(dynamic_cast<Slider*>(previous_selected_) != nullptr)
						{
							dynamic_cast<Slider*>(previous_selected_)->unselect();
						}
					}
					previous_selected_ = current_selected_;
					current_selected_ = ui;
				}
			}
			ui->handle_events(e);
		}
	}
}

void UiManager::draw(sdl::Renderer& renderer)
{
	for(std::unique_ptr<Ui> const& ui_element : ui_elements_)
	{
		ui_element->draw(renderer);
	}
}

void UiManager::update()
{
	for(std::unique_ptr<Ui> const& ui_element : ui_elements_)
	{
		ui_element->update();
		if(ui_element->state_ == State::SELECTED)
		{
			previous_selected_ = current_selected_;
			current_selected_ = ui_element.get();
		}
	}
}