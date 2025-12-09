#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>

namespace sdl
{

class Chunk
{
	public:
		explicit Chunk(const std::string_view file); //Mix_LoadWAV
		Chunk(const Chunk& chunk) = delete;
		Chunk& operator=(const Chunk& chunk) = delete;
		~Chunk(); //Mix_FreeChunk

		Mix_Chunk* fetch() const;
		void fade_in(const int channel, const int loops, const int ms) const;
		void fade_out(const int which, const int ms) const;
		void pause(const int channel) const;
		void resume(const int channel) const;
		void volume(const int volume) const;

	private:
		Mix_Chunk* chunk_;
};
}
