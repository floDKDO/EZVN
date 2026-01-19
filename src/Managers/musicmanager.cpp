#include "Managers/musicmanager.h"

MusicManager::MusicManager(AudioManager& audio_manager)
	: audio_properties_({}), music_(nullptr), audio_manager_(audio_manager)
{}

void MusicManager::fade_in(Music& music, Script::AudioProperties& music_properties)
{
	audio_manager_.fade_in_music(music, music_properties.loop_, music_properties.fadein_length_);
}

void MusicManager::play(Script::InfoMusic& info_music)
{
	const Script::AudioProperties& music_properties = info_music.first;
	Music& music = info_music.second.value();
	if(!sdl::music::playing())
	{
		audio_manager_.fade_in_music(music, music_properties.loop_, music_properties.fadein_length_);
	}
	else if(music_ != nullptr && music_ != &music)
	{
		audio_manager_.fade_out_music(music_properties.fadeout_length_);
	}
	audio_properties_ = music_properties;
	music_ = &music;
}

void MusicManager::stop(Script::InfoMusic& info_music)
{
	const Script::AudioProperties& music_properties = info_music.first;
	audio_manager_.fade_out_music(music_properties.fadeout_length_);
	music_ = nullptr;
}

void MusicManager::stop(int fadeout_length)
{
	audio_manager_.fade_out_music(fadeout_length);
	music_ = nullptr;
}

void MusicManager::update(Script::InfoMusic& info_music)
{
	//Musics
	if(info_music.second.has_value()) //play music
	{
		play(info_music);
	}
	else //stop music
	{
		stop(info_music);
	}
}