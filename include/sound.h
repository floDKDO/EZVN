#pragma once

#include "RAII_SDL2/chunk.h"

class Sound
{
	public:
		explicit Sound(const std::string path);

		void play_sound(const bool loop=false, const int fadein_length=0); 
		void pause_sound();
		void resume_sound();
		void stop_sound(const int fadeout_length = 0); 
		void change_volume(const int volume);

		static int global_sound_volume;

	protected:

	private:
		int channel;
		bool loop;
		std::string path;
		sdl::Chunk sound;
		int local_sound_volume; //TODO

		static int unused_channel;
};

