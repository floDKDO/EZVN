#pragma once

#include "RAII_SDL2/renderer.h"

enum class TransitionName
{
	fade,
	dissolve,
	wipeleft_scene
};

class Transition
{
	public:
		Transition(TransitionName transition_name);

		void draw(sdl::Renderer& renderer);
		void update();

	private:
		TransitionName transition_name_;
};

