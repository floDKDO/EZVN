#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>

class Music
{
	public:
		Music(const std::string path);
		~Music();
		Music(const Music& m);
		Music& operator=(Music m);

		friend void swap(Music& m1, Music& m2)
		{
			using std::swap;

			swap(m1.name, m2.name);
			swap(m1.loop, m2.loop);
			swap(m1.path, m2.path);
			swap(m1.music, m2.music);
		}

		void play_music(const bool loop = false, const int fadein_length = 0); //unité = s
		void pause_music() const;
		void resume_music() const;
		void stop_music(const int fadeout_length=0) const; //unité = s
		void change_volume(const int volume) const; //unité = pourcentage
		void set_position(const double position) const; //unité = seconde

	protected:

	private:
		std::string name;
		bool loop;
		std::string path;
		Mix_Music* music;
};

