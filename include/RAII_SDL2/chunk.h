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
		static void play_channel(sdl::Chunk& chunk, int channel, const bool loop, const int volume = MIX_MAX_VOLUME); //not the same as Mix_PlayChannel() => loop is a bool (true <=> -1, false <=> 0). It also has a additional parameter (volume)
		static void fade_in(sdl::Chunk& chunk, const int channel, const bool loop, const int ms, const int volume = MIX_MAX_VOLUME); //not the same as Mix_FadeInChannel() => loop is a bool (true <=> -1, false <=> 0). It also has a additional parameter (volume)
		static void fade_out(const int which, const int ms);
		static void halt_channel(int channel);
		static void pause(const int channel);
		static void resume(const int channel);
		static void volume(sdl::Chunk& chunk, const int volume);

	private:
		Mix_Chunk* chunk_;
};
}
