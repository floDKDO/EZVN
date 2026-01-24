#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>

namespace sdl
{

class Music
{
	public:
		explicit Music(std::string_view file); //Mix_LoadMUS
		Music(const Music& music) = delete;
		Music(Music&& music);
		Music& operator=(const Music& music) = delete;
		Music& operator=(Music&& music);
		~Music(); //Mix_FreeMusic

		Mix_Music* fetch() const;

	private:
		Mix_Music* music_;
};

	namespace music
	{

	bool playing();
	void fade_in(const sdl::Music& music, bool loop, int ms, int volume = MIX_MAX_VOLUME); //not the same as Mix_FadeInMusic() => loop is a bool (true <=> -1, false <=> 0). It also has a additional parameter (volume)
	void fade_out(int ms);
	void halt();
	void pause();
	void resume();
	void volume(int volume);
	void set_position(double position);
	void hook_finished(void(*music_finished)());

	}

}
