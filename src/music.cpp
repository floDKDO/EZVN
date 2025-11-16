#include "music.h"

int Music::global_music_volume = MIX_MAX_VOLUME / 2;

Music::Music(const std::string path)
	: loop(false), path(path), music(path)
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
	/*if(this->local_music_volume != global_music_volume) //Priority for the local music volume
	{
		this->change_volume(local_music_volume);
	}
	else*/
	{
		this->change_volume(global_music_volume);
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
	this->local_music_volume = volume;
	this->music.volume(volume * MIX_MAX_VOLUME / 100);
}

void Music::set_position(const double position) 
{
	this->music.set_position(position);
}