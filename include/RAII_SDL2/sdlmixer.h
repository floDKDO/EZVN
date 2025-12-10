#pragma once

#include <SDL2/SDL_mixer.h>

namespace sdl
{

class SDLMixer
{
	public:
		explicit SDLMixer(const int flags);
		SDLMixer(const SDLMixer& sdl_mixer) = delete;
		SDLMixer(SDLMixer&& sdl_mixer) = delete;
		SDLMixer& operator=(const SDLMixer& sdl_mixer) = delete;
		SDLMixer& operator=(SDLMixer&& sdl_mixer) = delete;
		~SDLMixer();

};
}
