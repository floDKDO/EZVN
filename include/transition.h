#pragma once

#include "RAII_SDL2/renderer.h"

class Transition
{
	public:
		enum class TransitionName
		{
			FADE,
			DISSOLVE,
			WIPELEFT_SCENE
		};

		Transition(TransitionName transition_name);

		//void draw(sdl::Renderer& renderer);
		//void update();

	private:
		TransitionName transition_name_;
};

