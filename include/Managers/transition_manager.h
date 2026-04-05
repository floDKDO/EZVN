#pragma once

#include "script.h"
#include "transition.h"

class TransitionManager
{
	public:
		TransitionManager();

		bool is_first_part_finished() const;
		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update(const Script::InfoTransition& info_transition);
		void update_transition();
		void reset();

		bool transition_playing_;
		bool new_background_displayed_;

	private:
		std::unique_ptr<Transition> transition_;
};

