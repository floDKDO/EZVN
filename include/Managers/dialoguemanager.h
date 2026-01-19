#pragma once

#include "textbox.h"
#include "script.h"

class DialogueManager
{
	public:
		enum class Where
		{
			next,
			prev,
			none
		};

		struct WhichDialogueFromWhere
		{
			Where which_dialogue_;
			bool is_from_mouse_wheel_;
			bool wait_for_end_of_dialogue_;
		};

		DialogueManager(sdl::Renderer& renderer);

		void handle_events_mouse_wheel(const SDL_Event& e);
		void handle_events_keyboard_mouse(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);

		void update_skip_auto_modes();
		void update(Script::InfoDialogue& info_dialogue, const Character& character);

		void reset();

		WhichDialogueFromWhere which_dialogue_from_where_;

	private:
		Uint64 last_time_;
		bool skip_mode_;
		bool auto_mode_;
		Textbox textbox_;
		sdl::Renderer& renderer_;
};

