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
		Chunk(Chunk&& chunk);
		Chunk& operator=(const Chunk& chunk) = delete;
		Chunk& operator=(Chunk&& chunk);
		~Chunk(); //Mix_FreeChunk

		Mix_Chunk* fetch() const;
		static bool playing(int channel);
		void fade_in(const int channel, const int loops, const int ms) const;
		static void fade_out(const int which, const int ms);
		static void halt_channel(int channel);
		static void pause(const int channel);
		static void resume(const int channel);
		void volume(const int volume) const;

	private:
		Mix_Chunk* chunk_;
};
}
