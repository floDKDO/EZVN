#include "Managers/ui_manager.h"
#include "GUI/slider.h"
#include "GUI/inputfield.h"
#include "GUI/scrollbar.h"
//#include "GUI/confirmation_popup.h"
#include "constants.h"
#include "utils.h"
#include "GUI/ui_group.h"

#include <iostream>

UiManager::UiManager(AudioManager& audio_manager)
	: is_mouse_on_widget_(false), previous_selected_(nullptr), current_selected_(nullptr), is_mouse_left_button_held_down_(false),
	audio_manager_(audio_manager), ui_select_sound_({sdl::Chunk{constants::ui_sound_select_}, 1.0}), ui_press_sound_({sdl::Chunk{constants::ui_sound_press_}, 1.0}), last_time_(0)
{}

void UiManager::reset()
{
	for(std::unique_ptr<Ui>& ui : ui_elements_)
	{
		ui.reset();
	}
	ui_elements_.clear();

	navigation_list_.clear();
}

void UiManager::add_element(std::unique_ptr<Ui> ui)
{
	ui_elements_.push_back(std::move(ui));
}

/*Ui* UiManager::get_last_element()
{
	return ui_elements_.back().get();
}*/

void UiManager::set_elements()
{
	for(const std::unique_ptr<Ui>& ui : ui_elements_)
	{
		std::vector<UiWidget*> nodes;
		if(UiWidget* widget = dynamic_cast<UiWidget*>(ui.get()); widget != nullptr)
		{
			nodes = widget->get_navigation_nodes();
		}
		else if(UiGroup* ui_group = dynamic_cast<UiGroup*>(ui.get()); ui_group != nullptr)
		{
			nodes = ui_group->get_navigation_nodes();
		}

		for(UiWidget* widget : nodes)
		{
			navigation_list_.push_back(widget);
		}

		/*TextButton* textbutton = nullptr; 
		if((textbutton = dynamic_cast<TextButton*>(ui.get())) != nullptr && textbutton->confirmationpopup_ != nullptr)
		{
			std::vector<Ui*> textbuttons_popup = textbutton->confirmationpopup_->get_navigation_nodes(); 
			navigation_list_.insert(navigation_list_.end(), textbuttons_popup.begin(), textbuttons_popup.end()); //Also add the textbuttons of the ConfirmationPopUp
		}*/
	}

	assign_widget_on_moving();

	for(UiWidget* widget : navigation_list_)
	{
		if(widget->state_ == UiWidget::State::SELECTED)
		{
			current_selected_ = widget;
			return;
		}
	}

	if(navigation_list_.size() > 0)
	{
		current_selected_ = navigation_list_[0];
		current_selected_->state_ = UiWidget::State::SELECTED;
	}
	else
	{
		current_selected_ = nullptr;
	}
}

bool UiManager::is_widget1_facing_widget2(SDL_Rect pos_ui1, SDL_Rect pos_ui2, Axis mode) const
{
	SDL_assert(mode == Axis::X || mode == Axis::Y);

	if(mode == Axis::X)
	{
		return (pos_ui1.x >= pos_ui2.x && pos_ui1.x <= pos_ui2.x + pos_ui2.w)
			|| (pos_ui1.x + pos_ui1.w >= pos_ui2.x && pos_ui1.x + pos_ui1.w <= pos_ui2.x + pos_ui2.w);
	}
	else if(mode == Axis::Y)
	{
		return (pos_ui1.y >= pos_ui2.y && pos_ui1.y <= pos_ui2.y + pos_ui2.h)
			|| (pos_ui1.y + pos_ui1.h >= pos_ui2.y && pos_ui1.y + pos_ui1.h <= pos_ui2.y + pos_ui2.h);
	}
	else
	{
		return false;
	}
}

bool UiManager::is_candidate_closer(const UiWidget* widget, const UiWidget* candidate_widget, const UiWidget* current_best_widget, Axis mode) const
{
	SDL_assert(mode == Axis::X || mode == Axis::Y);

	const int diff_best_x = std::abs(widget->rect_.x - current_best_widget->rect_.x);
	const int diff_candidate_x = std::abs(widget->rect_.x - candidate_widget->rect_.x);
	const int diff_best_y = std::abs(widget->rect_.y - current_best_widget->rect_.y);
	const int diff_candidate_y = std::abs(widget->rect_.y - candidate_widget->rect_.y);

	if(mode == Axis::X)
	{
		return diff_best_x > diff_candidate_x || (diff_best_x == diff_candidate_x && diff_best_y > diff_candidate_y);
	}
	else if(mode == Axis::Y)
	{
		return diff_best_y > diff_candidate_y || (diff_best_y == diff_candidate_y && diff_best_x > diff_candidate_x);
	}
	else
	{
		return false;
	}
}

//widget = widget to which we assign a "select_on_*"
//candidate = the current widget in the inner for loop
//current_best = the "select_on_*" we try to assign to widget (up, down, left or right)
UiWidget* UiManager::get_widget_facing(const UiWidget* widget, UiWidget* candidate_widget, UiWidget* current_best_widget, Axis mode) const
{
	UiWidget* new_best_widget = nullptr;
	if(current_best_widget == nullptr)
	{
		new_best_widget = candidate_widget;
	}
	else
	{
		const bool widget_facing_current_best = is_widget1_facing_widget2(widget->rect_, current_best_widget->rect_, mode);
		const bool widget_facing_candidate = is_widget1_facing_widget2(widget->rect_, candidate_widget->rect_, mode);
		const bool widget_facing_both = widget_facing_current_best && widget_facing_candidate;
		const bool widget_facing_neither = !widget_facing_current_best && !widget_facing_candidate;

		//if the two widgets are facing each other or if neither of them are facing each other, take the one with the lowest diff* 
		//if current_best is not facing widget but candidate_widget is facing widget, take candidate_widget
		if(widget_facing_both || widget_facing_neither)
		{
			if(is_candidate_closer(widget, candidate_widget, current_best_widget, mode))
			{
				new_best_widget = candidate_widget;
			}
		}
		else if(!widget_facing_current_best && widget_facing_candidate)
		{
			new_best_widget = candidate_widget;
		}
	}

	if(new_best_widget == nullptr)
	{
		new_best_widget = current_best_widget;
	}

	return new_best_widget;
}

void UiManager::assign_widget_on_moving() const
{
	for(UiWidget* widget : navigation_list_)
	{
		UiWidget* current_best_up = nullptr;
		UiWidget* current_best_down = nullptr;
		UiWidget* current_best_left = nullptr;
		UiWidget* current_best_right = nullptr;

		for(UiWidget* candidate : navigation_list_)
		{
			if(candidate == widget) { continue; }

			const SDL_Rect candidate_pos = candidate->rect_;
			const SDL_Rect widget_pos = widget->rect_;

			if(candidate_pos.y + candidate_pos.h <= widget_pos.y)
			{
				current_best_up = get_widget_facing(widget, candidate, current_best_up, Axis::X);
			}
			if(candidate_pos.y >= widget_pos.y + widget_pos.h)
			{
				current_best_down = get_widget_facing(widget, candidate, current_best_down, Axis::X);
			}
			if(candidate_pos.x + candidate_pos.w <= widget_pos.x)
			{
				current_best_left = get_widget_facing(widget, candidate, current_best_left, Axis::Y);
			}
			if(candidate_pos.x >= widget_pos.x + widget_pos.w)
			{
				current_best_right = get_widget_facing(widget, candidate, current_best_right, Axis::Y);
			}
		}
		widget->select_on_up_ = current_best_up;
		widget->select_on_down_ = current_best_down;
		widget->select_on_left_ = current_best_left;
		widget->select_on_right_ = current_best_right;
	}
}

void UiManager::select_new(UiWidget* widget)
{
	audio_manager_.play_sound(ui_select_sound_, -1, false);
	
	previous_selected_ = current_selected_;
	previous_selected_->state_ = UiWidget::State::NORMAL;

	current_selected_ = widget;
	current_selected_->state_ = UiWidget::State::SELECTED;
}

void UiManager::unselect_previous(UiWidget* widget, PointerEventData pointer_event_data)
{
	if(is_mouse_on_specific_widget(widget, pointer_event_data))
	{
		if(current_selected_ != widget && !is_mouse_left_button_held_down_)
		{
			previous_selected_ = current_selected_;
			previous_selected_->state_ = UiWidget::State::NORMAL;
			current_selected_ = widget;
			if(Slider* slider = dynamic_cast<Slider*>(previous_selected_); slider != nullptr)
			{
				slider->disable_keyboard_focus();
			}
			else if(Inputfield* inputfield = dynamic_cast<Inputfield*>(previous_selected_); inputfield != nullptr)
			{
				inputfield->quit_editing();
			}
			else if(Scrollbar* scrollbar = dynamic_cast<Scrollbar*>(previous_selected_); scrollbar != nullptr)
			{
				scrollbar->disable_keyboard_focus();
			}
		}
	}
}

bool UiManager::is_mouse_on_widget(PointerEventData pointer_event_data)
{
	bool result = false;
	for(UiWidget* widget : navigation_list_)
	{
		result = is_mouse_on_specific_widget(widget, pointer_event_data);
	}
	is_mouse_on_widget_ = false;
	return result;
}

bool UiManager::is_mouse_on_specific_widget(UiWidget* widget, PointerEventData pointer_event_data)
{
	std::vector<UiWidget*> all_widgets = widget->get_navigation_nodes();

	for(size_t i = 0; i < all_widgets.size(); ++i)
	{
		const SDL_Rect& rect = all_widgets[i]->rect_;

		if(utils::is_point_in_rect({pointer_event_data.mouse_x_, pointer_event_data.mouse_y_}, rect))
		{
			is_mouse_on_widget_ = true;
			return true;
		}
	}
	is_mouse_on_widget_ = false;
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
	for(UiWidget* widget : navigation_list_)
	{
		if(e.type == SDL_MOUSEMOTION)
		{
			PointerEventData pointer_event_data = {SDL_BUTTON_LEFT, e.motion.x, e.motion.y};
			unselect_previous(widget, pointer_event_data);
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
				if(is_mouse_on_specific_widget(current_selected_, pointer_event_data) && !is_mouse_left_button_held_down_)
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

				if(is_mouse_left_button_held_down_ && (current_selected_->state_ == UiWidget::State::SELECTED || current_selected_->state_ == UiWidget::State::PRESSED)) //cas "state == State::SELECTED" uniquement pour pouvoir bouger la poignée du Slider sans que la souris soit sur la poignée
				{
					is_mouse_left_button_held_down_ = false;
					if((current_selected_->pointer_on_ui_when_pointer_up_ && is_mouse_on_specific_widget(current_selected_, pointer_event_data)) //first condition: the callback function is called only if the pointer is on the UI when it is released/up
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
				if(is_mouse_on_specific_widget(current_selected_, pointer_event_data))
				{
					current_selected_->mouse_entered_ = true;
					if(!is_mouse_left_button_held_down_ && current_selected_->state_ == UiWidget::State::NORMAL)
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

		case SDL_MOUSEWHEEL:
			{
				//PointerEventData pointer_event_data = {0, e.motion.x, e.motion.y};
				//if(is_mouse_on_specific_widget(current_selected_, pointer_event_data))
				{
					//std::cout << "EVENT MOUSE WHEEL\n";
					current_selected_->on_mouse_wheel(e.wheel.y);
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
	for(const std::unique_ptr<Ui>& ui : ui_elements_) 
	{
		ui->draw(renderer);
	}
}

void UiManager::update()
{
	for(const std::unique_ptr<Ui>& ui : ui_elements_) 
	{
		ui->update();
	}
}