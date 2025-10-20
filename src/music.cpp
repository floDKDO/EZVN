#include "music.h"


Music::Music(const std::string path)
	: name(path), loop(false)
{
	this->music = Mix_LoadMUS(path.c_str());
}

Music::~Music()
{
	Mix_FreeMusic(this->music);
}

void Music::play_music(const bool loop, const int fadein_length)
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

void Music::pause_music() const
{
	Mix_PauseMusic();
}

void Music::resume_music() const
{
	Mix_ResumeMusic();
}

void Music::stop_music(const int fadeout_length) const
{
	Mix_FadeOutMusic(fadeout_length * 1000);
}

void Music::change_volume(const int volume) const //[0; MIX_MAX_VOLUME(=128)]
{
	//0 = 0%, 128 = 100%
	Mix_VolumeMusic(volume * MIX_MAX_VOLUME / 100);
}

void Music::set_position(const double position) const 
{
	Mix_SetMusicPosition(position);
}