#include "Managers/uimanager.h"
#include "GUI/checkablegroup.h"
#include "GUI/texttoggle.h"
#include "GUI/slider.h"
#include "GUI/inputfield.h"
#include "GUI/confirmationpopup.h"
#include "constants.h"
#include "game.h"

#include <iostream>

UiManager::UiManager(AudioManager& audio_manager)
	: is_mouse_on_ui_(false), previous_selected_(nullptr), current_selected_(nullptr), is_mouse_left_button_held_down_(false),
	audio_manager_(audio_manager), ui_select_sound_({sdl::Chunk{constants::ui_sound_select_}, 1.0}), ui_press_sound_({sdl::Chunk{constants::ui_sound_press_}, 1.0}), last_time_(0)
{}

//TODO : rvalue ref ??
void UiManager::add_element(std::unique_ptr<Ui>&& ui)
{
	ui_elements_.push_back(std::move(ui));
}

void UiManager::set_elements()
{
	for(const std::unique_ptr<Ui>& ui_element : ui_elements_)
	{
		std::vector<Ui*> nodes = ui_element->get_navigation_nodes();

		navigation_list_.insert(navigation_list_.end(), nodes.begin(), nodes.end());

		/*TextButton* textbutton = nullptr; 
		if((textbutton = dynamic_cast<TextButton*>(ui_element.get())) != nullptr && textbutton->confirmationpopup_ != nullptr)
		{
			std::vector<Ui*> textbuttons_popup = textbutton->confirmationpopup_->get_navigation_nodes(); 
			navigation_list_.insert(navigation_list_.end(), textbuttons_popup.begin(), textbuttons_popup.end()); //Also add the textbuttons of the ConfirmationPopUp
		}*/
	}

	assign_ui_on_moving();

	for(Ui* ui : navigation_list_)
	{
		if(ui->state_ == State::SELECTED)
		{
			current_selected_ = ui;
			return;
		}
	}
	current_selected_ = navigation_list_[0];
	current_selected_->state_ = State::SELECTED;
}

bool UiManager::is_ui1_facing_ui2(SDL_Rect pos_ui1, SDL_Rect pos_ui2, Axis mode) const
{
	SDL_assert(mode == Axis::X_AXIS || mode == Axis::Y_AXIS);

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
	else
	{
		return false;
	}
}

bool UiManager::is_candidate_closer(const Ui* ui, const Ui* candidate, const Ui* current_best, Axis mode) const
{
	SDL_assert(mode == Axis::X_AXIS || mode == Axis::Y_AXIS);

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
	else
	{
		return false;
	}
}

//ui = ui to which we assign a "select_on_*"
//candidate = the current ui in the inner for loop
//current_best = the "select_on_*" we try to assign to ui (up, down, left or right)
Ui* UiManager::get_ui_facing(const Ui* ui, Ui* candidate, Ui* current_best, Axis mode) const
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
	return current_best;
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
				current_best_up = get_ui_facing(ui, candidate, current_best_up, Axis::X_AXIS);
			}
			if(candidate_pos.y >= ui_pos.y + ui_pos.h)
			{
				current_best_down = get_ui_facing(ui, candidate, current_best_down, Axis::X_AXIS);
			}
			if(candidate_pos.x + candidate_pos.w <= ui_pos.x)
			{
				current_best_left = get_ui_facing(ui, candidate, current_best_left, Axis::Y_AXIS);
			}
			if(candidate_pos.x >= ui_pos.x + ui_pos.w)
			{
				current_best_right = get_ui_facing(ui, candidate, current_best_right, Axis::Y_AXIS);
			}
		}
		ui->select_on_up_ = current_best_up;
		ui->select_on_down_ = current_best_down;
		ui->select_on_left_ = current_best_left;
		ui->select_on_right_ = current_best_right;
	}
}

void UiManager::select_new(Ui* ui)
{
	audio_manager_.play_sound(ui_select_sound_, -1, false);
	
	previous_selected_ = current_selected_;
	previous_selected_->state_ = State::NORMAL;

	current_selected_ = ui;
	current_selected_->state_ = State::SELECTED;
}

void UiManager::unselect_previous(Ui* ui, PointerEventData pointer_event_data)
{
	if(is_mouse_on_specific_ui(ui, pointer_event_data))
	{
		if(current_selected_ != ui && !is_mouse_left_button_held_down_)
		{
			previous_selected_ = current_selected_;
			previous_selected_->state_ = State::NORMAL;
			current_selected_ = ui;
			if(dynamic_cast<Slider*>(previous_selected_) != nullptr)
			{
				dynamic_cast<Slider*>(previous_selected_)->disable_keyboard_focus();
			}
			else if(dynamic_cast<Inputfield*>(previous_selected_) != nullptr)
			{
				dynamic_cast<Inputfield*>(previous_selected_)->quit_editing();
			}
		}
	}
}

bool UiManager::is_mouse_on_ui(PointerEventData pointer_event_data)
{
	bool result = false;
	for(Ui* ui_element : navigation_list_)
	{
		result = is_mouse_on_specific_ui(ui_element, pointer_event_data);
	}
	is_mouse_on_ui_ = false;
	return result;
}

bool UiManager::is_mouse_on_specific_ui(Ui* ui, PointerEventData pointer_event_data)
{
	std::vector<Ui*> all_ui = ui->get_navigation_nodes();

	for(size_t i = 0; i < all_ui.size(); ++i)
	{
		const SDL_Rect& rect = all_ui[i]->get_rect();

		if(rect.y + rect.h > pointer_event_data.mouse_y
		&& rect.y < pointer_event_data.mouse_y
		&& rect.x + rect.w > pointer_event_data.mouse_x
		&& rect.x < pointer_event_data.mouse_x)
		{
			is_mouse_on_ui_ = true;
			return true;
		}
	}
	is_mouse_on_ui_ = false;
	return false;
}

void UiManager::on_input_pressed(const SDL_Event& e)
{
	if((e.type == SDL_CONTROLLERBUTTONDOWN && e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
			|| (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_UP))
	{
		current_selected_->on_up_pressed();
		if(!current_selected_->has_keyboard_focus_ && current_selected_->select_on_up_ != nullptr)
		{
			select_new(current_selected_->select_on_up_);
		}
	}
	else if((e.type == SDL_CONTROLLERBUTTONDOWN && e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		 || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_DOWN))
	{
		current_selected_->on_down_pressed();
		if(!current_selected_->has_keyboard_focus_ && current_selected_->select_on_down_ != nullptr)
		{
			select_new(current_selected_->select_on_down_);
		}
	}
	else if((e.type == SDL_CONTROLLERBUTTONDOWN && e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
		 || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_LEFT))
	{
		current_selected_->on_left_pressed();
		if(!current_selected_->has_keyboard_focus_ && current_selected_->select_on_left_ != nullptr)
		{
			select_new(current_selected_->select_on_left_);
		}
	}
	else if((e.type == SDL_CONTROLLERBUTTONDOWN && e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		 || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RIGHT))
	{
		current_selected_->on_right_pressed();
		if(!current_selected_->has_keyboard_focus_ && current_selected_->select_on_right_ != nullptr)
		{
			select_new(current_selected_->select_on_right_);
		}
	}
	else if((e.type == SDL_CONTROLLERBUTTONDOWN && e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
		 || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN))
	{
		current_selected_->on_enter_pressed();
	}
	else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE)
	{
		current_selected_->on_backspace_pressed();
	}
	else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_DELETE)
	{
		current_selected_->on_delete_pressed();
	}
}

void UiManager::on_input_released(const SDL_Event& e)
{
	if((e.type == SDL_CONTROLLERBUTTONUP && e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
			|| (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_UP))
	{
		current_selected_->on_up_released();
	}
	else if((e.type == SDL_CONTROLLERBUTTONUP && e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		 || (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_DOWN))
	{
		current_selected_->on_down_released();
	}
	else if((e.type == SDL_CONTROLLERBUTTONUP && e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
		 || (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_LEFT))
	{
		current_selected_->on_left_released();
	}
	else if((e.type == SDL_CONTROLLERBUTTONUP && e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		 || (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_RIGHT))
	{
		current_selected_->on_right_released();
	}
	else if((e.type == SDL_CONTROLLERBUTTONUP && e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
		 || (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_RETURN))
	{
		audio_manager_.play_sound(ui_press_sound_, -1, false);
		current_selected_->on_enter_released();
	}
}

void UiManager::handle_events(const SDL_Event& e)
{
	for(Ui* ui : navigation_list_)
	{
		if(e.type == SDL_MOUSEMOTION)
		{
			PointerEventData pointer_event_data = {SDL_BUTTON_LEFT, e.motion.x, e.motion.y};
			unselect_previous(ui, pointer_event_data);
		}
	}

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
				PointerEventData pointer_event_data = {SDL_BUTTON_LEFT, e.motion.x, e.motion.y};
				if(is_mouse_on_specific_ui(current_selected_, pointer_event_data) && !is_mouse_left_button_held_down_)
				{
					is_mouse_left_button_held_down_ = true;
					current_selected_->mouse_was_on_ui_before_drag_ = true;
					current_selected_->on_pointer_down(pointer_event_data);
				}
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if(e.button.button == SDL_BUTTON_LEFT)
			{
				PointerEventData pointer_event_data = {SDL_BUTTON_LEFT, e.motion.x, e.motion.y};

				if(is_mouse_left_button_held_down_ && (current_selected_->state_ == State::SELECTED || current_selected_->state_ == State::PRESSED)) //cas "state == State::SELECTED" uniquement pour pouvoir bouger la poignée du Slider sans que la souris soit sur la poignée
				{
					is_mouse_left_button_held_down_ = false;
					if((current_selected_->pointer_on_ui_when_pointer_up_ && is_mouse_on_specific_ui(current_selected_, pointer_event_data)) //first condition: the callback function is called only if the pointer is on the UI when it is released/up
					|| !current_selected_->pointer_on_ui_when_pointer_up_) //second conditon: the callback function is called even if the pointer is not on the UI when it is released/up
					{
						audio_manager_.play_sound(ui_press_sound_, -1, false);
						current_selected_->on_pointer_up(pointer_event_data);
						current_selected_->mouse_was_on_ui_before_drag_ = false;
					}
				}
			}
			break;

		case SDL_MOUSEMOTION:
			{
				PointerEventData pointer_event_data = {0, e.motion.x, e.motion.y};
				if(is_mouse_on_specific_ui(current_selected_, pointer_event_data))
				{
					current_selected_->mouse_entered_ = true;
					if(!is_mouse_left_button_held_down_ && current_selected_->state_ == State::NORMAL)
					{
						audio_manager_.play_sound(ui_select_sound_, -1, false);
						current_selected_->on_pointer_enter(pointer_event_data);
					}
				}
				else
				{
					if(current_selected_->mouse_entered_)
					{
						current_selected_->mouse_entered_ = false;
						current_selected_->on_pointer_exit(pointer_event_data);
					}
				}

				if(is_mouse_left_button_held_down_ && current_selected_->mouse_was_on_ui_before_drag_)
				{
					current_selected_->on_drag(pointer_event_data);
				}
			}
			break;

		case SDL_TEXTINPUT:
			if(current_selected_->wants_text_input_)
			{
				current_selected_->on_typing(e.text.text);
			}
			break;

		default:
			break;
	}
}

void UiManager::draw(sdl::Renderer& renderer)
{
	for(const std::unique_ptr<Ui>& ui_element : ui_elements_) 
	{
		ui_element->draw(renderer);
	}
}

void UiManager::update()
{
	for(const std::unique_ptr<Ui>& ui_element : ui_elements_)  
	{
		ui_element->update();
	}
}