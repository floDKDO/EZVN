#pragma once

#include "RAII_SDL2/chunk.h"

class Sound
{
	public:
		explicit Sound(const std::string path);

		void play_sound(const bool loop=false, const int fadein_length=0); 
		void pause_sound() const;
		void resume_sound() const;
		void stop_sound(const int fadeout_length = 0) const;
		void change_volume(const int volume);

		static int global_sound_volume_;

	private:
		static int unused_channel_;

		sdl::Chunk sound_;
		bool loop_;
		std::string path_;
		int local_sound_volume_; //TODO
		int channel_;
};

