#include "RAII_SDL2/sdlmixer.h"

namespace sdl
{

SDLMixer::SDLMixer(int flags)
{
	Mix_Init(flags);
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_CHANNELS, 2048);
}

SDLMixer::~SDLMixer()
{
	Mix_CloseAudio();
	Mix_Quit();
}

}