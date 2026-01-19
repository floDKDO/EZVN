#pragma once

#include "script.h"
#include "charactermanager.h"
#include "dialoguemanager.h"
#include "textbox.h"
#include "audiomanager.h"
#include "RAII_SDL2/channel.h"

class ScriptRunner
{
	public:
		struct ScriptIndexWhenPrev
		{
			bool is_saved_;
			size_t saved_script_index_;
		};

		struct MusicManager
		{
			explicit MusicManager(AudioManager& audio_manager)
				: audio_properties_({}), music_(nullptr), audio_manager_(audio_manager)
			{}

			void update(Script::InfoMusic& info_music)
			{
				//Musics
				const Script::AudioProperties& music_properties = info_music.first;
				if(info_music.second.has_value()) //play music
				{
					Music& music = info_music.second.value();
					if(!sdl::music::playing())
					{
						audio_manager_.fade_in_music(music, music_properties.loop_, music_properties.fadein_length_);
					}
					else if(music_ != nullptr && music_ != &music)
					{
						std::cout << "FADEOUT\n";
						audio_manager_.fade_out_music(music_properties.fadeout_length_);
					}
					audio_properties_ = music_properties;
					music_ = &music;
				}
				else //stop music
				{
					audio_manager_.fade_out_music(music_properties.fadeout_length_);
					music_ = nullptr;
				}
			}

			struct Script::AudioProperties audio_properties_;
			Music* music_;

			AudioManager& audio_manager_;
		};

		struct SoundManager
		{
			explicit SoundManager(AudioManager& audio_manager)
				: audio_properties_({}), played_(false), sound_(nullptr), audio_manager_(audio_manager)
			{}

			void update(Script::InfoSound& info_sound)
			{
				Script::AudioProperties& sound_properties = info_sound.first;

				if(info_sound.second.has_value()) //play sound
				{
					Sound& sound = info_sound.second.value();

					if(!sdl::channel::playing(sound_properties.channel_)
					&& ((!sound_properties.loop_ && !played_) || sound_properties.loop_)) //ne pas rejouer un son qui a déjà été joué s'il ne doit pas être joué en boucle
					{
						std::cout << "FADEIN SON\n";
						audio_manager_.fade_in_sound(sound, sound_properties.channel_, sound_properties.loop_, sound_properties.fadein_length_);
						played_ = true;
					}
					else if(sound_ != nullptr && sound_ != &sound)
					{
						if(sdl::channel::playing(sound_properties.channel_))
						{
							std::cout << "FADEOUT SON\n";
							audio_manager_.fade_out_sound(sound_properties.channel_, sound_properties.fadeout_length_);
							played_ = false;
						}
						else
						{
							std::cout << "FADEOUT FADEIN SON\n";
							audio_manager_.fade_in_sound(sound, sound_properties.channel_, sound_properties.loop_, sound_properties.fadein_length_);
							played_ = true;
						}
					}
					audio_properties_ = sound_properties;
					sound_ = &sound;
				}
				else //stop sound
				{
					if(sdl::channel::playing(sound_properties.channel_))
					{
						audio_manager_.fade_out_sound(sound_properties.channel_, sound_properties.fadeout_length_);
						sound_ = nullptr;
						played_ = false;
					}
				}
			}

			struct Script::AudioProperties audio_properties_;
			bool played_;
			Sound* sound_;

			AudioManager& audio_manager_;
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

		struct TextboxManager
		{
			TextboxManager(sdl::Renderer& renderer)
				: textbox_(renderer)
			{}

			void update(Script::InfoTextbox& info_textbox)
			{

			}

			Textbox textbox_;
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

		ScriptRunner(Script& script, AudioManager& audio_manager, CharacterManager& character_manager, sdl::Renderer& renderer);

		void increment_script_index();
		void decrement_script_index();
		bool is_current_script_index_a_dialogue();
		bool move_dialogue(DialogueManager::Where where, bool is_from_mouse_wheel_);

		void handle_events(const SDL_Event& e);
		void draw(sdl::Renderer& renderer);
		void apply_line(size_t script_index);
		void update();
		void rebuild();

		size_t current_script_index_;
		size_t previous_script_index_;
		ScriptIndexWhenPrev script_index_when_prev_;

	private:
		Script& script_;

		bool init_;

		CharacterManager& character_manager_;
		BackgroundManager background_manager_;
		MusicManager music_manager_;  
		SoundManager sound_manager_;
		TextboxManager textbox_manager_;
		DialogueManager dialogue_manager_;
		AutofocusManager autofocus_manager_;
};

