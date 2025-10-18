#include "music.h"


Music::Music(std::string path)
	: name(path), loop(false)
{
	this->music = Mix_LoadMUS(path.c_str());
}

Music::~Music()
{
	Mix_FreeMusic(this->music);
}

void Music::play_music(bool loop, int fadein_length)
{
	int loops;
	if(loop)
	{
		loops = -1;
		this->loop = true;
	}
	else
	{
		loops = 0;
		this->loop = false;
	}

	Mix_FadeInMusic(this->music, loops, fadein_length * 1000);
}

void Music::pause_music()
{
	Mix_PauseMusic();
}

void Music::resume_music()
{
	Mix_ResumeMusic();
}

void Music::stop_music(int fadeout_length)
{
	Mix_FadeOutMusic(fadeout_length * 1000);
}

void Music::change_volume(int volume) //[0; MIX_MAX_VOLUME(=128)]
{
	//0 = 0%, 128 = 100%
	Mix_VolumeMusic(volume * 128 / 100);
}

void Music::set_position(double position)
{
	Mix_SetMusicPosition(position);
}