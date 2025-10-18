#pragma once

#include <string>
#include <SDL2/SDL_mixer.h>

class Music
{
	public:
	Music(std::string path);
	~Music();
	void play_music(bool loop = false, int fadein_length = 0); //unité = s
	void pause_music();
	void resume_music();
	void stop_music(int fadeout_length=0); //unité = s
	void change_volume(int volume); //unité = pourcentage
	void set_position(double position); //unité = seconde

	std::string name;
	Mix_Music* music;
	bool loop;

	protected:

	private:

};

