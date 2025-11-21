#include "RAII_SDL2/chunk.h"

namespace sdl
{

Chunk::Chunk(const std::string file) //Mix_LoadWAV
{
	chunk_ = Mix_LoadWAV(file.c_str());
}

Chunk::~Chunk() //Mix_FreeChunk
{
	Mix_FreeChunk(chunk_);
}

Mix_Chunk* Chunk::Get() const
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
