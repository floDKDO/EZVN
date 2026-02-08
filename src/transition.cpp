#include "transition.h"
#include "utils.h"

#include <iostream>

Transition::Transition(const Script::InfoTransition& info_transition)
	: info_transition_(info_transition), first_part_finished_(false), second_part_finished_(false), transition_finished_(false), rect_({0, 0, 1280, 720}), alpha_(0), last_time_(SDL_GetTicks64()), t_(0.0f)
{}

void Transition::draw(sdl::Renderer& renderer)
{
	if(info_transition_.transition_name_ == "fade")
	{
		renderer.set_draw_color(0, 0, 0, alpha_);
		renderer.fill_rect(&rect_);
	}
}

void Transition::update()
{
	if(info_transition_.transition_name_ == "fade")
	{
		fade();
	}
}

void Transition::fade()
{
	if(!transition_finished_) //TODO : à terme, il faudrait utiliser des TransformStep
	{
		Uint64 now = SDL_GetTicks64();
		if(!first_part_finished_)
		{
			Uint8 new_alpha = Uint8(utils::lerp(0, 255, t_));
			alpha_ = new_alpha;

			if(info_transition_.length_to_black_.has_value())
			{
				t_ = float(now - last_time_) / float(info_transition_.length_to_black_.value());
			}
			else
			{
				t_ = float(now - last_time_) / float(constants::default_fade_transition_length_);
			}

			if(t_ > 1.0f)
			{
				t_ = 0.0f;
				first_part_finished_ = true;
				last_time_ = SDL_GetTicks64();

				if(!info_transition_.length_black_.has_value()) //cas default => pas d'attente dans l'écran noir
				{
					second_part_finished_ = true;
				}
			}
		}
		else if(!second_part_finished_)
		{
			if(SDL_GetTicks64() - last_time_ > info_transition_.length_black_.value()) //attendre x ms dans l'écran noir
			{
				second_part_finished_ = true;
				last_time_ = SDL_GetTicks64();
			}
		}
		else
		{
			Uint8 new_alpha = Uint8(utils::lerp(255, 0, t_));
			alpha_ = new_alpha;

			if(info_transition_.length_to_black_.has_value())
			{
				t_ = float(now - last_time_) / float(info_transition_.length_to_clear_.value());
			}
			else
			{
				t_ = float(now - last_time_) / float(constants::default_fade_transition_length_);
			}

			if(t_ > 1.0f)
			{
				t_ = 0.0f;
				transition_finished_ = true;
				last_time_ = SDL_GetTicks64();
			}
		}
	}
}