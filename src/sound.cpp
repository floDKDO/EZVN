#include "sound.h"

int Sound::global_sound_volume_ = MIX_MAX_VOLUME / 2;
int Sound::unused_channel_ = 0;

Sound::Sound(const std::string path)
	: sound_(path), loop_(false), path_(path), local_sound_volume_(global_sound_volume_), channel_(unused_channel_)
{
	unused_channel_ += 1;
	if(unused_channel_ == MIX_CHANNELS)
	{
		unused_channel_ = 0;
	}
}

void Sound::play_sound(const bool loop, const int fadein_length)
{
	int loops;
	if(loop)
	{
		loops = -1;
		loop_ = true;
	}
	else
	{
		loops = 0;
		loop_ = false;
	}
	/*if(local_sound_volume != global_sound_volume) //Priority for the local sound volume
	{
		change_volume(local_sound_volume);
	}
	else*/
	{
		change_volume(global_sound_volume_);
	}
	sound_.fade_in(channel_, loops, fadein_length);
}

void Sound::pause_sound() const
{
	sound_.pause(channel_);
}

void Sound::resume_sound() const
{
	sound_.resume(channel_);
}

void Sound::stop_sound(const int fadeout_length) const
{
	sound_.fade_out(channel_, fadeout_length);
}

void Sound::change_volume(const int volume) //[0; MIX_MAX_VOLUME(=128)]
{
	//0 = 0%, 128 = 100%
	local_sound_volume_ = volume;
	sound_.volume(volume * MIX_MAX_VOLUME / 100);
}