#include "RAII_SDL2/music.h"

#include <SDL2/SDL.h>

namespace sdl
{

Music::Music(const std::string file) //Mix_LoadMUS
{
	if((music_ = Mix_LoadMUS(file.c_str())) == nullptr)
	{
		SDL_Log("(Mix_LoadMUS) %s\n", Mix_GetError());
	}
}

Music::~Music() //Mix_FreeMusic
{
	Mix_FreeMusic(music_);
}

Mix_Music* Music::fetch() const
{
	return music_;
}

void Music::fade_in(const int loops, const int ms)
{
	if(Mix_FadeInMusic(music_, loops, ms) == -1)
	{
		SDL_Log("(Mix_FadeInMusic) %s\n", Mix_GetError());
	}
}

void Music::fade_out(const int ms)
{
	Mix_FadeOutMusic(ms);
}

void Music::pause()
{
	Mix_PauseMusic();
}

void Music::resume()
{
	Mix_ResumeMusic();
}

void Music::volume(const int volume)
{
	Mix_VolumeMusic(volume);
}

void Music::set_position(const double position)
{
	if(Mix_SetMusicPosition(position) == -1)
	{
		SDL_Log("(Mix_SetMusicPosition) %s\n", Mix_GetError());
	}
}

}