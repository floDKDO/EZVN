#include "sound.h"

int Sound::unused_channel = 0;

Sound::Sound(const std::string path)
	: name(path), channel(this->unused_channel), loop(false)
{
	this->sound = Mix_LoadWAV(path.c_str());
	this->unused_channel += 1;
	if(this->unused_channel == MIX_CHANNELS)
	{
		this->unused_channel = 0;
	}
}

Sound::~Sound()
{
	Mix_FreeChunk(this->sound);
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
	Mix_FadeInChannel(this->channel, this->sound, loops, fadein_length);
}

void Sound::pause_sound() const
{
	Mix_Pause(this->channel);
}

void Sound::resume_sound() const
{
	Mix_Resume(this->channel);
}

void Sound::stop_sound(const int fadeout_length) const
{
	Mix_FadeOutChannel(this->channel, fadeout_length);
}

void Sound::change_volume(const int volume) const //[0; MIX_MAX_VOLUME(=128)]
{
	//0 = 0%, 128 = 100%
	Mix_VolumeChunk(this->sound, volume * MIX_MAX_VOLUME / 100);
}