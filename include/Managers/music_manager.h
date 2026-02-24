#pragma once

#include "Managers/audio_manager.h"
#include "script.h"

class MusicManager
{
	public:
		explicit MusicManager(AudioManager& audio_manager);

		void fade_in(Music& music, Script::AudioProperties& music_properties);
		void fade_out(Script::AudioProperties& music_properties);
		void play(Script::InfoMusic& info_music);
		void stop(Script::InfoMusic& info_music);
		void stop(int fadeout_length);
		void update(Script::InfoMusic& info_music);

		Script::AudioProperties audio_properties_;
		Music* music_;

	private:
		AudioManager& audio_manager_;
};

