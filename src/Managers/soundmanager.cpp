#include "Managers/soundmanager.h"
#include "RAII_SDL2/channel.h"

SoundManager::SoundManager(AudioManager& audio_manager)
	: audio_properties_({}), played_(false), sound_(nullptr), audio_manager_(audio_manager)
{}

void SoundManager::reset()
{
	sound_ = nullptr;
	played_ = false;
	audio_properties_ = {};
	sdl::channel::halt_channel(-1); //si scroll arrière, stopper tous les sons
}

void SoundManager::fade_in(Sound& sound, Script::AudioProperties& sound_properties)
{
	audio_manager_.fade_in_sound(sound, sound_properties.channel_, sound_properties.loop_, sound_properties.fadein_length_);
	played_ = true;
}

void SoundManager::fade_out(Script::AudioProperties& sound_properties)
{
	audio_manager_.fade_out_sound(sound_properties.channel_, sound_properties.fadeout_length_);
	played_ = false;
}

void SoundManager::play(Script::InfoSound& info_sound)
{
	Script::AudioProperties& sound_properties = info_sound.first;
	Sound& sound = info_sound.second.value();

	if(!sdl::channel::playing(sound_properties.channel_)
	&& ((!sound_properties.loop_ && !played_) || sound_properties.loop_)) //ne pas rejouer un son qui a déjà été joué s'il ne doit pas être joué en boucle
	{
		fade_in(sound, sound_properties);
	}
	else if(sound_ != nullptr && sound_ != &sound)
	{
		if(sdl::channel::playing(sound_properties.channel_))
		{
			fade_out(sound_properties);
		}
		else
		{
			fade_in(sound, sound_properties);
		}
	}
	audio_properties_ = sound_properties;
	sound_ = &sound;
}

void SoundManager::stop(Script::InfoSound& info_sound)
{
	Script::AudioProperties& sound_properties = info_sound.first;
	if(sdl::channel::playing(sound_properties.channel_))
	{
		fade_out(sound_properties);
	}
	sound_ = nullptr;
	audio_properties_ = {};
}

void SoundManager::update(Script::InfoSound& info_sound)
{
	if(info_sound.second.has_value()) //play sound
	{
		play(info_sound);
	}
	else //stop sound
	{
		stop(info_sound);
	}
}