#pragma once

#include "Managers/character_manager.h"
#include "Managers/textbox_manager.h"
#include "Managers/music_manager.h"
#include "Managers/sound_manager.h"
#include "Managers/background_manager.h"
#include "Managers/transition_manager.h"
#include "Managers/choice_menu_manager.h"

#include <set>

class ScriptRunner
{
	public:
		ScriptRunner(Game& game, sdl::Renderer& renderer);

		void init_dialogues_script_index();
		std::optional<size_t> get_script_index_of_previous_dialogue();
		size_t get_script_index_of_first_dialogue();

		void increment_script_index();
		void decrement_script_index();
		bool is_current_script_index_a_dialogue();
		bool is_script_index_a_dialogue(size_t script_index);
		void save_current_script_index_when_scroll_back();
		void move_dialogue(TextboxManager::Where where, bool is_from_mouse_wheel_);
		void init_to_first_dialogue();

		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void apply_line(size_t script_index);
		void update();
		void rebuild();
		void play_all_sounds_before_previous_dialogue(size_t target_script_index);
		void handle_music_when_scroll_back(size_t target_script_index);

		size_t current_script_index_;

	private:
		struct ScriptIndexWhenPrev
		{
			bool is_saved_;
			size_t saved_script_index_;
		};
		ScriptIndexWhenPrev script_index_when_prev_;

		Script& script_;

		std::set<size_t> dialogues_script_index_;

		bool init_;
		bool is_choice_menu_visible_;
		bool is_dialogue_of_choice_menu_visible_;

		CharacterManager character_manager_;
		BackgroundManager background_manager_;
		MusicManager music_manager_;  
		SoundManager sound_manager_;
		TextboxManager textbox_manager_;
		TransitionManager transition_manager_;
		ChoiceMenuManager choice_menu_manager_;
};

