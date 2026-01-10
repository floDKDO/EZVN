#include "audiomanager.h"

int AudioManager::global_music_volume_ = MIX_MAX_VOLUME;
int AudioManager::global_sound_volume_ = MIX_MAX_VOLUME;

AudioManager::AudioManager()
	: current_music_(nullptr)
{}

void AudioManager::fade_in_music(Music& music, bool loop, int ms)
{
	current_music_ = &music;
	int volume = int(music.volume_multiplier_ * global_music_volume_);
	sdl::Music::volume(volume);
	sdl::Music::fade_in(current_music_->music_, loop, ms, volume);
}

void AudioManager::fade_out_music(int ms)
{
	sdl::Music::fade_out(ms);
	current_music_ = nullptr;
}

void AudioManager::set_volume_music(int volume)
{
	global_music_volume_ = volume;
	sdl::Music::volume(int(current_music_->volume_multiplier_ * global_music_volume_));
}

void AudioManager::play_sound(Sound& sound, int channel, bool loop)
{
	int volume = int(sound.volume_multiplier_ * global_sound_volume_);
	sdl::Chunk::volume_chunk(sound.sound_, volume);
	sdl::Chunk::play_channel(sound.sound_, channel, loop, volume);
}

void AudioManager::fade_in_sound(Sound& sound, int channel, bool loop, int ms)
{
	int volume = int(sound.volume_multiplier_ * global_sound_volume_);
	sdl::Chunk::volume_chunk(sound.sound_, volume);
	sdl::Chunk::fade_in(sound.sound_, channel, loop, ms, volume);
}

void AudioManager::fade_out_sound(int channel, int ms)
{
	sdl::Chunk::fade_out(channel, ms);
}

void AudioManager::set_volume_channel(int channel, int volume)
{
	global_sound_volume_ = volume;
	sdl::Chunk::volume(channel, global_sound_volume_);
}