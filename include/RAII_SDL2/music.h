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
		Music& operator=(const Music& music) = delete;
		~Music(); //Mix_FreeMusic

		Mix_Music* fetch() const;
		void fade_in(const int loops, const int ms) const;
		void fade_out(const int ms) const;
		void pause() const;
		void resume() const;
		void volume(const int volume) const;
		void set_position(const double position) const;

	private:
		Mix_Music* music_;
};
}
