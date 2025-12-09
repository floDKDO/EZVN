#include "music.h"

int Music::global_music_volume_ = MIX_MAX_VOLUME / 2;

Music::Music(const std::string_view path)
	: loop_(false), music_(path), local_music_volume_(global_music_volume_)
{}

void Music::play_music(const bool loop, const int fadein_length)
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
	/*if(local_music_volume != global_music_volume) //Priority for the local music volume
	{
		change_volume(local_music_volume);
	}
	else*/
	{
		change_volume(global_music_volume_);
	}
	music_.fade_in(loops, fadein_length * 1000);
}

void Music::pause_music() const
{
	music_.pause();
}

void Music::resume_music() const
{
	music_.resume();
}

void Music::stop_music(const int fadeout_length) const
{
	music_.fade_out(fadeout_length * 1000);
}

void Music::change_volume(const int volume) //[0; MIX_MAX_VOLUME(=128)]
{
	//0 = 0%, 128 = 100%
	local_music_volume_ = volume;
	music_.volume(volume * MIX_MAX_VOLUME / 100);
}

void Music::set_position(const double position) const
{
	music_.set_position(position);
}