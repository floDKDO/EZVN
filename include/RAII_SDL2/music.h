#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>

namespace sdl
{

class Music
{
	public:
		explicit Music(const std::string file); //Mix_LoadMUS
		Music(const Music& music) = delete;
		Music& operator=(const Music& music) = delete;
		~Music(); //Mix_FreeMusic

		Mix_Music* fetch() const;
		void fade_in(const int loops, const int ms);
		void fade_out(const int ms);
		void pause();
		void resume();
		void volume(const int volume);
		void set_position(const double position);

	protected:

	private:
		Mix_Music* music_;
};
}
