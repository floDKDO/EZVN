#include "RAII_SDL2/sdl_mixer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

namespace sdl
{

SDLMixer::SDLMixer(int flags)
{
	if(Mix_Init(flags) == 0)
	{
		SDL_Log("(Mix_Init) %s\n", Mix_GetError());
	}

	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_CHANNELS, 2048) == -1)
	{
		SDL_Log("(Mix_OpenAudio) %s\n", Mix_GetError());
	}
}

SDLMixer::~SDLMixer()
{
	Mix_CloseAudio();
	Mix_Quit();
}

}