#pragma once

#include "RAII_SDL2/music.h"

class Music
{
	public:
		explicit Music(const std::string path);

		void play_music(const bool loop = false, const int fadein_length = 0); //unité = s
		void pause_music();
		void resume_music();
		void stop_music(const int fadeout_length=0); //unité = s
		void change_volume(const int volume); //unité = pourcentage
		void set_position(const double position); //unité = seconde

		static int global_music_volume;

	protected:

	private:
		bool loop;
		std::string path;
		sdl::Music music;
		int local_music_volume; //TODO
};

 