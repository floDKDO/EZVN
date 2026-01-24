#pragma once

#include <SDL2/SDL_mixer.h>
#include <string_view>

namespace sdl
{

class Chunk
{
	public:
		explicit Chunk(std::string_view file); //Mix_LoadWAV
		Chunk(const Chunk& chunk) = delete;
		Chunk(Chunk&& chunk);
		Chunk& operator=(const Chunk& chunk) = delete;
		Chunk& operator=(Chunk&& chunk);
		~Chunk(); //Mix_FreeChunk

		Mix_Chunk* fetch() const;
		void volume_chunk(int volume); 

	private:
		Mix_Chunk* chunk_;
};

}
