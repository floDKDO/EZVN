#include "Managers/audio_manager.h"
#include "RAII_SDL2/channel.h"
#include "RAII_SDL2/events.h"

#include <SDL2/SDL.h>

int AudioManager::global_music_volume_ = MIX_MAX_VOLUME;
int AudioManager::global_sound_volume_ = MIX_MAX_VOLUME;
const Uint32 AudioManager::END_MUSIC_EVENT_ = sdl::events::register_events(1); 
const Uint32 AudioManager::END_CHANNEL_EVENT_ = sdl::events::register_events(1);

AudioManager::AudioManager()
	: current_music_(nullptr)
{
	sdl::music::hook_finished(&callback_music); 
	sdl::channel::channel_finished(&callback_channel);
}

void AudioManager::callback_music()
{
	SDL_Event e = {END_MUSIC_EVENT_}; //init le premier élément de l'union (= type)
	sdl::events::push_event(&e);
}

void AudioManager::callback_channel([[maybe_unused]] int channel)
{
	SDL_Event e = {END_CHANNEL_EVENT_};
	sdl::events::push_event(&e);
}

void AudioManager::fade_in_music(Music& music, bool loop, int ms)
{
	current_music_ = &music;
	int volume = int(music.volume_multiplier_ * global_music_volume_);
	sdl::music::volume(volume);
	sdl::music::fade_in(current_music_->music_, loop, ms, volume);
}

void AudioManager::fade_out_music(int ms)
{
	sdl::music::fade_out(ms);
	current_music_ = nullptr;
}

void AudioManager::set_volume_music(int volume)
{
	global_music_volume_ = volume;
	if(current_music_ != nullptr)
	{
		sdl::music::volume(int(current_music_->volume_multiplier_ * global_music_volume_));
	}
}

void AudioManager::play_sound(Sound& sound, int channel, bool loop)
{
	int volume = int(sound.volume_multiplier_ * global_sound_volume_);
	sound.sound_.volume_chunk(volume);
	sdl::channel::play_channel(sound.sound_, channel, loop, volume);
}

void AudioManager::fade_in_sound(Sound& sound, int channel, bool loop, int ms)
{
	int volume = int(sound.volume_multiplier_ * global_sound_volume_);
	sound.sound_.volume_chunk(volume);
	sdl::channel::fade_in(sound.sound_, channel, loop, ms, volume);
}

void AudioManager::fade_out_sound(int channel, int ms)
{
	sdl::channel::fade_out(channel, ms);
}

void AudioManager::set_volume_channel(int channel, int volume)
{
	global_sound_volume_ = volume;
	sdl::channel::volume(channel, global_sound_volume_);
}

void AudioManager::halt_channel(int channel)
{
	sdl::channel::halt_channel(channel);
}