#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>

class Sound
{
	public:
		Sound(const std::string path);
		~Sound();
		Sound(const Sound& s);
		Sound& operator=(Sound s);

		friend void swap(Sound& s1, Sound& s2)
		{
			using std::swap;

			swap(s1.name, s2.name);
			swap(s1.channel, s2.channel);
			swap(s1.loop, s2.loop);
			swap(s1.path, s2.path);
			swap(s1.sound, s2.sound);
		}

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

