#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>

#include "SDL/music.h"

class Music
{
	public:
		Music(const std::string path);

		void play_music(const bool loop = false, const int fadein_length = 0); //unité = s
		void pause_music();
		void resume_music();
		void stop_music(const int fadeout_length=0); //unité = s
		void change_volume(const int volume); //unité = pourcentage
		void set_position(const double position); //unité = seconde

	protected:

	private:
		std::string name;
		bool loop;
		std::string path;
		sdl::Music music;
};

