#pragma once

#include <SDL2/SDL_mixer.h>

namespace sdl
{
	class SDLMixer;
}

class sdl::SDLMixer
{
	public:
		SDLMixer(int flags);
		SDLMixer(const SDLMixer& sdl) = delete;
		SDLMixer& operator=(const SDLMixer& sdl) = delete;
		~SDLMixer();

	protected:

	private:

};

