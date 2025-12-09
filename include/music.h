#pragma once

#include "RAII_SDL2/music.h"

class Music
{
	public:
		explicit Music(const std::string_view path);

		void play_music(const bool loop = false, const int fadein_length = 0); //unité = s
		void pause_music() const;
		void resume_music() const;
		void stop_music(const int fadeout_length=0) const; //unité = s
		void change_volume(const int volume); //unité = pourcentage
		void set_position(const double position) const; //unité = seconde

		static int global_music_volume_;

	private:
		bool loop_;
		sdl::Music music_;
		int local_music_volume_; //TODO
};

 