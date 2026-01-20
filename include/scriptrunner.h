#pragma once

#include "Managers/charactermanager.h"
#include "Managers/textboxmanager.h"
#include "Managers/musicmanager.h"
#include "Managers/soundmanager.h"
#include "Managers/backgroundmanager.h"

class ScriptRunner
{
	public:
		struct ScriptIndexWhenPrev
		{
			bool is_saved_;
			size_t saved_script_index_;
		};

		ScriptRunner(Game& game, sdl::Renderer& renderer);

		void increment_script_index();
		void decrement_script_index();
		bool is_current_script_index_a_dialogue();
		bool move_dialogue(TextboxManager::Where where, bool is_from_mouse_wheel_);

		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void apply_line(size_t script_index);
		void update();
		void rebuild();
		void play_all_sounds_before_previous_dialogue(size_t target_script_index);
		void handle_music_when_scroll_back(size_t target_script_index);

		size_t current_script_index_;
		size_t previous_script_index_;
		ScriptIndexWhenPrev script_index_when_prev_;

	private:
		Script& script_;

		bool init_;

		CharacterManager character_manager_;
		BackgroundManager background_manager_;
		MusicManager music_manager_;  
		SoundManager sound_manager_;
		TextboxManager textbox_manager_;
};

