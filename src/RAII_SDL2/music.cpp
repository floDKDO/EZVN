#include "RAII_SDL2/music.h"

namespace sdl
{

Music::Music(const std::string file) //Mix_LoadMUS
{
	this->music = Mix_LoadMUS(file.c_str());
}

Music::~Music() //Mix_FreeMusic
{
	Mix_FreeMusic(this->music);
}

Mix_Music* Music::Get() const
{
	return this->music;
}

void Music::fade_in(int loops, int ms)
{
	Mix_FadeInMusic(this->music, loops, ms);
}

void Music::fade_out(int ms)
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

void Music::volume(int volume)
{
	Mix_VolumeMusic(volume);
}

void Music::set_position(double position)
{
	Mix_SetMusicPosition(position);
}

}