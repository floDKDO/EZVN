#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>

namespace sdl
{
	class Music;
}

class sdl::Music
{
	public:
		explicit Music(const std::string file); //Mix_LoadMUS
		Music(const Music& music) = delete;
		Music& operator=(const Music& music) = delete;
		~Music(); //Mix_FreeMusic

		Mix_Music* Get() const;
		void fade_in(int loops, int ms);
		void fade_out(int ms);
		void pause();
		void resume();
		void volume(int volume);
		void set_position(double position);

	protected:

	private:
		Mix_Music* music;
};

