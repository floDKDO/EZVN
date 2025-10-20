#pragma once

#include <string>
#include <SDL2/SDL_mixer.h>

class Music
{
	public:
	Music(const std::string path);
	~Music();
	void play_music(const bool loop = false, const int fadein_length = 0); //unit� = s
	void pause_music() const;
	void resume_music() const;
	void stop_music(const int fadeout_length=0) const; //unit� = s
	void change_volume(const int volume) const; //unit� = pourcentage
	void set_position(const double position) const; //unit� = seconde

	std::string name;
	Mix_Music* music;
	bool loop;

	protected:

	private:

};

