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

		static int global_sound_volume_;

	protected:

	private:
		int channel_;
		bool loop_;
		std::string path_;
		sdl::Chunk sound_;
		int local_sound_volume_; //TODO

		static int unused_channel_;
};

