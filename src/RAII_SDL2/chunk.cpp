#include "RAII_SDL2/chunk.h"

#include <SDL2/SDL.h>
#include <iostream>

namespace sdl
{

Chunk::Chunk(const std::string_view file) //Mix_LoadWAV
{
	if((chunk_ = Mix_LoadWAV(file.data())) == nullptr)
	{
		SDL_Log("(Mix_LoadWAV) %s\n", Mix_GetError());
	}
}

Chunk::Chunk(Chunk&& chunk)
	: chunk_(chunk.chunk_)
{
	chunk.chunk_ = nullptr;
}

Chunk& Chunk::operator=(Chunk&& chunk)
{
	if(this == &chunk)
	{
		return *this;
	}

	if(chunk_ != nullptr)
	{
		Mix_FreeChunk(chunk_);
	}

	chunk_ = chunk.chunk_;
	chunk.chunk_ = nullptr;
	return *this;
}

Chunk::~Chunk() //Mix_FreeChunk
{
	if(chunk_ != nullptr)
	{
		Mix_FreeChunk(chunk_);
	}
}

Mix_Chunk* Chunk::fetch() const
{
	return chunk_;
}

bool Chunk::playing(int channel)
{
	return Mix_Playing(channel);
}

void Chunk::play_channel(sdl::Chunk& chunk, int channel, const bool loop, const int volume)
{
	int loops;
	if(loop)
	{
		loops = -1;
	}
	else
	{
		loops = 0;
	}

	Chunk::volume_chunk(chunk, volume);

	Mix_PlayChannel(channel, chunk.fetch(), loops);
}

void Chunk::fade_in(sdl::Chunk& chunk, const int channel, const bool loop, const int ms, const int volume)
{
	int loops;
	if(loop)
	{
		loops = -1;
	}
	else
	{
		loops = 0;
	}

	Chunk::volume_chunk(chunk, volume);

	Mix_FadeInChannel(channel, chunk.fetch(), loops, ms);
}

void Chunk::fade_out(const int which, const int ms)
{
	Mix_FadeOutChannel(which, ms);
}

void Chunk::halt_channel(int channel)
{
	if(Mix_HaltChannel(channel) == -1)
	{
		SDL_Log("(Mix_HaltChannel) %s\n", Mix_GetError());
	}
}

void Chunk::pause(const int channel)
{
	Mix_Pause(channel);
}

void Chunk::resume(const int channel)
{
	Mix_Resume(channel);
}

void Chunk::volume_chunk(sdl::Chunk& chunk, const int volume)
{
	Mix_VolumeChunk(chunk.fetch(), volume);
}

void Chunk::volume(int channel, const int volume)
{
	Mix_Volume(channel, volume);
}

}
