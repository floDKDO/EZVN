#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>

class Sound
{
	public:
		Sound(const std::string path);
		~Sound();
		Sound(const Sound& s);
		Sound& operator=(const Sound& s);

		void play_sound(const bool loop=false, const int fadein_length=0); 
		void pause_sound() const;
		void resume_sound() const;
		void stop_sound(const int fadeout_length = 0) const; 
		void change_volume(const int volume) const;

	protected:

	private:
		std::string name;
		int channel;
		bool loop;
		std::string path;
		Mix_Chunk* sound;

		static int unused_channel;
};

