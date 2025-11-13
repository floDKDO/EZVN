#include "sound.h"

int Sound::unused_channel = 0;

Sound::Sound(const std::string path)
	: channel(this->unused_channel), loop(false), path(path), sound(path)
{
	this->unused_channel += 1;
	if(this->unused_channel == MIX_CHANNELS)
	{
		this->unused_channel = 0;
	}
}

void Sound::play_sound(const bool loop, const int fadein_length)
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
	this->sound.fade_in(this->channel, loops, fadein_length);
}

void Sound::pause_sound()
{
	this->sound.pause(this->channel);
}

void Sound::resume_sound()
{
	this->sound.resume(this->channel);
}

void Sound::stop_sound(const int fadeout_length) 
{
	this->sound.fade_out(this->channel, fadeout_length);
}

void Sound::change_volume(const int volume) //[0; MIX_MAX_VOLUME(=128)]
{
	//0 = 0%, 128 = 100%
	this->sound.volume(volume * MIX_MAX_VOLUME / 100);
}