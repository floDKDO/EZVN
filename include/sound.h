#pragma once

#include <string>
#include <SDL2/SDL_mixer.h>

class Sound
{
	public:
		Sound(const std::string path);
		~Sound();
		void play_sound(const bool loop=false, const int fadein_length=0); 
		void pause_sound() const;
		void resume_sound() const;
		void stop_sound(const int fadeout_length = 0) const; 
		void change_volume(const int volume) const;

		std::string name;
		Mix_Chunk* sound;
		int channel;
		bool loop;

		static int unused_channel;

	protected:

	private:

};

