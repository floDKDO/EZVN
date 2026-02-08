#pragma once

#include "RAII_SDL2/renderer.h"
#include "transform_step.h"
#include "script.h"

#include <string>
#include <string_view>

/*
* Transitions : 
* ex : 
  show bg class_day
  s "..."
  scene bg house with wipeleft_scene // => passer de class_day à house avec un wipeleft_scene
*
* Les personnages et la textbox apparaissent dans le bg house quand la transition wipeleft_scene est terminée
* Comme la textbox n'apparaît pas pendant une transition, on ne peut pas aller dans Settings ou appuyer sur Skip 
* Comme avec les dialogues, rappuyer sur le clic gauche ou la touche espace termine immédiatement la transition
* 
*/

class Transition
{
	public:
		Transition(const Script::InfoTransition& info_transition);

		void draw(sdl::Renderer& renderer);
		void update();
		void fade();

		Script::InfoTransition info_transition_;
		bool first_part_finished_;
		bool second_part_finished_;
		bool transition_finished_;

	private:
		SDL_Rect rect_;
		Uint8 alpha_;
		Uint64 last_time_;
		float t_;
};

