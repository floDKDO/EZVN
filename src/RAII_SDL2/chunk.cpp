#include "RAII_SDL2/chunk.h"

#include <SDL2/SDL.h>

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

void Chunk::fade_in(const int channel, int loops, int ms) const
{
	Mix_FadeInChannel(channel, chunk_, loops, ms);
}

void Chunk::fade_out(const int which, const int ms) const
{
	Mix_FadeOutChannel(which, ms);
}

void Chunk::pause(const int channel) const
{
	Mix_Pause(channel);
}

void Chunk::resume(const int channel) const
{
	Mix_Resume(channel);
}

void Chunk::volume(const int volume) const
{
	Mix_VolumeChunk(chunk_, volume);
}

}
