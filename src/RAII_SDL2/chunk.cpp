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

void Chunk::volume_chunk(const int volume)
{
	Mix_VolumeChunk(chunk_, volume);
}

}
