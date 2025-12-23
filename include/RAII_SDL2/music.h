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
		void fade_in(const int loops, const int ms) const;
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
