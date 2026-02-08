#pragma once

#include "script.h"
#include "transition.h"

class TransitionManager
{
	public:
		TransitionManager();

		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void update(const Script::InfoTransition& info_transition);
		void update_transition();
		void reset();

		bool transition_playing_;
		std::unique_ptr<Transition> transition_;
		bool new_background_displayed_;

	private:
};

