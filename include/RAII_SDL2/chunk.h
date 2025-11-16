#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>

namespace sdl
{
	class Chunk;
}

class sdl::Chunk
{
	public:
		explicit Chunk(const std::string file); //Mix_LoadWAV
		Chunk(const Chunk& chunk) = delete;
		Chunk& operator=(const Chunk& chunk) = delete;
		~Chunk(); //Mix_FreeChunk

		Mix_Chunk* Get() const;
		void fade_in(int channel, int loops, int ms);
		void fade_out(int which, int ms);
		void pause(int channel);
		void resume(int channel);
		void volume(int volume);

	protected:

	private:
		Mix_Chunk* chunk;
};

