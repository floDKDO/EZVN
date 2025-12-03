#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>

namespace sdl
{

class Chunk
{
	public:
		explicit Chunk(const std::string file); //Mix_LoadWAV
		Chunk(const Chunk& chunk) = delete;
		Chunk& operator=(const Chunk& chunk) = delete;
		~Chunk(); //Mix_FreeChunk

		Mix_Chunk* fetch() const;
		void fade_in(const int channel, const int loops, const int ms);
		void fade_out(const int which, const int ms);
		void pause(const int channel);
		void resume(const int channel);
		void volume(const int volume);

	protected:

	private:
		Mix_Chunk* chunk_;
};
}
