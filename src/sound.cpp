#include "sound.h"

int Sound::unused_channel = 0;

Sound::Sound(std::string path)
	: name(path), loop(false)
{
	this->sound = Mix_LoadWAV(path.c_str());
}

Sound::~Sound()
{
	Mix_FreeChunk(this->sound);
}

void Sound::play_sound(bool loop, int fadein_length)
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

void Sound::pause_sound()
{
	Mix_Pause(this->channel);
}

void Sound::resume_sound()
{
	Mix_Resume(this->channel);
}

void Sound::stop_sound(int fadeout_length)
{
	Mix_FadeOutChannel(this->channel, fadeout_length);
}

void Sound::change_volume(int volume) //[0; MIX_MAX_VOLUME(=128)]
{
	//0 = 0%, 128 = 100%
	Mix_VolumeChunk(this->sound, volume * 128 / 100);
}