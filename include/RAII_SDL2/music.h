#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>

namespace sdl
{

class Music
{
	public:
		explicit Music(const std::string_view file); //Mix_LoadMUS
		Music(const Music& music) = delete;
		Music(Music&& music);
		Music& operator=(const Music& music) = delete;
		Music& operator=(Music&& music);
		~Music(); //Mix_FreeMusic

		Mix_Music* fetch() const;
		static bool playing();
		static void fade_in(sdl::Music& music, const bool loop, const int ms, const int volume = MIX_MAX_VOLUME); //not the same as Mix_FadeInMusic() => loop is a bool (true <=> -1, false <=> 0). Is also has a additional parameter (volume)
		static void fade_out(const int ms);
		static void halt();
		static void pause();
		static void resume();
		static void volume(const int volume);
		static void set_position(const double position);

	private:
		Mix_Music* music_;
};
}
