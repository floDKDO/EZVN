#pragma once

#include "Managers/charactermanager.h"
#include "Managers/textboxmanager.h"
#include "Managers/musicmanager.h"
#include "Managers/soundmanager.h"

class ScriptRunner
{
	public:
		struct ScriptIndexWhenPrev
		{
			bool is_saved_;
			size_t saved_script_index_;
		};

		struct BackgroundManager
		{
			explicit BackgroundManager(sdl::Renderer& renderer)
				: background_(Color::from_rgba8(0, 0, 0)), renderer_(renderer)
			{}

			void draw(sdl::Renderer& renderer)
			{
				background_.draw(renderer);
			}

			void update(const Script::InfoBackground& info_background)
			{
				//Backgrounds
				if(info_background.image_ != nullptr)
				{
					if(background_.image_ != nullptr)
					{
						background_.image_->init_image(info_background.image_->path_, 0, 0, renderer_);
					}
					else
					{
						background_ = Background(info_background.image_->path_, renderer_);
					}
				}
				else
				{
					background_.image_.reset();
					background_.color_ = info_background.color_;
				}
			}

			Background background_;
			sdl::Renderer& renderer_;
		};

		struct AutofocusManager
		{
			AutofocusManager()
				: autofocus_(constants::default_autofocus)
			{}

			void update(const Script::InfoAutofocus& info_autofocus)
			{
				////if(!autofocus_changed_)
				//{
				//	if(info_autofocus || constants::default_autofocus)
				//	{
				//		/*for(auto& [key_character_variable, value_character] : active_characters_)
				//		{
				//			if(value_character.character_definition_->character_variable_ == textbox_.)
				//			{
				//				value_character.properties_.is_speaking_ = true;
				//				value_character.properties_.zorder_ = 10;
				//			}
				//			else
				//			{
				//				value_character.properties_.is_speaking_ = false;
				//				value_character.properties_.zorder_ = 5;
				//			}
				//		}*/
				//	}
				//	//autofocus_changed_ = true;
				//}
				autofocus_ = info_autofocus;
			}

			bool autofocus_;
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
		AutofocusManager autofocus_manager_;
};

