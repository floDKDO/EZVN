#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>

#include "SDL/chunk.h"

class Sound
{
	public:
		Sound(const std::string path);

		void play_sound(const bool loop=false, const int fadein_length=0); 
		void pause_sound();
		void resume_sound();
		void stop_sound(const int fadeout_length = 0); 
		void change_volume(const int volume);

	protected:

	private:
		std::string name;
		int channel;
		bool loop;
		std::string path;
		sdl::Chunk sound;

		static int unused_channel;
};

