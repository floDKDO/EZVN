#pragma once

#include <string>
#include <SDL2/SDL_mixer.h>

class Sound
{
	public:
		Sound(std::string path);
		~Sound();
		void play_sound(bool loop=false, int fadein_length=0); //TODO : loop, fadein/fadeout
		void pause_sound();
		void resume_sound();
		void stop_sound(int fadeout_length = 0); //TODO : fadein/fadeout
		void change_volume(int volume);

		std::string name;
		Mix_Chunk* sound;
		int channel;
		bool loop;

		static int unused_channel;

	protected:

	private:

};

