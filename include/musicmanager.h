#pragma once

#include "music.h"

#include <map>
#include <optional>

class MusicManager
{
	public:
		MusicManager(unsigned int& unique_id, unsigned int& current_unique_id);

		void insert_music(const std::string_view music_path, int fadein_length, int fadeout_length, int volume, bool loop);
		void update();

	private:
		struct AudioProperties
		{
			int fadein_length;
			int fadeout_length;
			int volume;
			bool loop;
			int channel; //not used for musics
		};

		std::map<unsigned int, std::pair<AudioProperties, std::optional<Music>>> musics_;
		Music* currently_playing_music_;
		unsigned int& unique_id_;
		unsigned int& current_unique_id_;
};

