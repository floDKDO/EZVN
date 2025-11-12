#include "music.h"

Music::Music(const std::string path)
	: name(path), loop(false), path(path), music(path)
{

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

	this->music.fade_in(loops, fadein_length * 1000);
}

void Music::pause_music() 
{
	this->music.pause();
}

void Music::resume_music() 
{
	this->music.resume();
}

void Music::stop_music(const int fadeout_length) 
{
	this->music.fade_out(fadeout_length * 1000);
}

void Music::change_volume(const int volume) //[0; MIX_MAX_VOLUME(=128)]
{
	//0 = 0%, 128 = 100%
	this->music.volume(volume * MIX_MAX_VOLUME / 100);
}

void Music::set_position(const double position) 
{
	this->music.set_position(position);
}