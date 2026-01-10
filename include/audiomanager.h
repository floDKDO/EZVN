#pragma once

#include "music.h"
#include "sound.h"

class AudioManager
{
	public:
		AudioManager();
		void fade_in_music(Music& music, bool loop, int ms);
		void fade_out_music(int ms);
		void set_volume_music(int volume);

		void play_sound(Sound& sound, int channel, bool loop);
		void fade_in_sound(Sound& sound, int channel, bool loop, int ms);
		void fade_out_sound(int channel, int ms);
		void set_volume_channel(int channel, int volume);

		static int global_music_volume_;
		static int global_sound_volume_;

	private:
		Music* current_music_;
};

