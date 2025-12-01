#include "RAII_SDL2/chunk.h"

#include <SDL2/SDL.h>

namespace sdl
{

Chunk::Chunk(const std::string file) //Mix_LoadWAV
{
	if((chunk_ = Mix_LoadWAV(file.c_str())) == nullptr)
	{
		SDL_Log("(Mix_LoadWAV) %s\n", Mix_GetError());
	}
}

Chunk::~Chunk() //Mix_FreeChunk
{
	Mix_FreeChunk(chunk_);
}

Mix_Chunk* Chunk::fetch() const
{
	return chunk_;
}

void Chunk::fade_in(int channel, int loops, int ms)
{
	Mix_FadeInChannel(channel, chunk_, loops, ms);
}

void Chunk::fade_out(int which, int ms)
{
	Mix_FadeOutChannel(which, ms);
}

void Chunk::pause(int channel)
{
	Mix_Pause(channel);
}

void Chunk::resume(int channel)
{
	Mix_Resume(channel);
}

void Chunk::volume(int volume)
{
	Mix_VolumeChunk(chunk_, volume);
}

}
