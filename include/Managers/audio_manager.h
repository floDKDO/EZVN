#pragma once

#include "music.h"
#include "sound.h"

class AudioManager
{
	public:
		AudioManager();

		static void callback_music();
		static void callback_channel(int channel);

		void fade_in_music(Music& music, bool loop, int ms);
		void fade_out_music(int ms);
		void set_volume_music(int volume);
		
		void play_sound(Sound& sound, int channel, bool loop);
		void fade_in_sound(Sound& sound, int channel, bool loop, int ms);
		void fade_out_sound(int channel, int ms);
		void set_volume_channel(int channel, int volume);
		void halt_channel(int channel);

		static int global_music_volume_;
		static int global_sound_volume_;
		static const Uint32 END_MUSIC_EVENT_; //majuscule pour coller avec la syntaxe des events de SDL2
		static const Uint32 END_CHANNEL_EVENT_; //majuscule pour coller avec la syntaxe des events de SDL2

	private:
		Music* current_music_;
};

