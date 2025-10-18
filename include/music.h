#pragma once

#include <string>
#include <SDL2/SDL_mixer.h>

class Music
{
	public:
	Music(std::string path);
	~Music();
	void play_music(bool loop = false, int fadein_length = 0); //unit� = s
	void pause_music();
	void resume_music();
	void stop_music(int fadeout_length=0); //unit� = s
	void change_volume(int volume); //unit� = pourcentage
	void set_position(double position); //unit� = seconde

	std::string name;
	Mix_Music* music;
	bool loop;

	protected:

	private:

};

