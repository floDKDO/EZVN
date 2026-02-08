#include "Managers/transition_manager.h"

#include <iostream>

TransitionManager::TransitionManager()
	: transition_playing_(false), new_background_displayed_(false)
{}

void TransitionManager::handle_events(const SDL_Event& e)
{
	if((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
	|| (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT))
	{
		reset(); //terminate immediately the transition if the player presses space or left click
	}
}

void TransitionManager::draw(sdl::Renderer& renderer)
{
	if(transition_ != nullptr)
	{
		transition_->draw(renderer);
	}
}

void TransitionManager::update(const Script::InfoTransition& info_transition)
{
	if(transition_ == nullptr)
	{
		transition_ = std::make_unique<Transition>(info_transition);
	}
	transition_playing_ = true;
}

void TransitionManager::update_transition()
{
	if(transition_ != nullptr)
	{
		transition_->update();
		if(transition_->first_part_finished_ && !transition_->transition_finished_)
		{
			//if(!one_time_)
			{
				//std::cout << "FIRST PART FINISHED\n";
				//current_script_index_ += 1;
				//new_background_displayed_ = true;
			}
		}
		else if(transition_->transition_finished_)
		{
			reset();
		}
	}
}

void TransitionManager::reset()
{
	transition_.reset();
	transition_ = nullptr;
	new_background_displayed_ = false;
	transition_playing_ = false;
}