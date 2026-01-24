#pragma once

#include "Managers/audio_manager.h"
#include "script.h"

class SoundManager
{
	public:
		explicit SoundManager(AudioManager& audio_manager);

		void reset();
		void fade_in(Sound& sound, Script::AudioProperties& sound_properties);
		void fade_out(Script::AudioProperties& sound_properties);
		void play(Script::InfoSound& info_sound);
		void stop(Script::InfoSound& info_sound);
		void update(Script::InfoSound& info_sound);

		Script::AudioProperties audio_properties_;
		bool played_;
		Sound* sound_;

	private:
		AudioManager& audio_manager_;
};

