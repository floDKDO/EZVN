#include "RAII_SDL2/channel.h"

#include <SDL2/SDL.h>

namespace sdl::channel
{

bool playing(int channel)
{
	return Mix_Playing(channel);
}

void play_channel(sdl::Chunk& chunk, int channel, bool loop, int volume)
{
	int loops;
	if(loop)
	{
		loops = -1;
	}
	else
	{
		loops = 0;
	}

	sdl::channel::volume(channel, volume);

	Mix_PlayChannel(channel, chunk.fetch(), loops);
}

void fade_in(sdl::Chunk& chunk, int channel, bool loop, int ms, int volume)
{
	int loops;
	if(loop)
	{
		loops = -1;
	}
	else
	{
		loops = 0;
	}

	sdl::channel::volume(channel, volume);

	Mix_FadeInChannel(channel, chunk.fetch(), loops, ms);
}

void fade_out(int which, int ms)
{
	Mix_FadeOutChannel(which, ms);
}

void halt_channel(int channel)
{
	if(Mix_HaltChannel(channel) == -1)
	{
		SDL_Log("(Mix_HaltChannel) %s\n", Mix_GetError());
	}
}

void pause(int channel)
{
	Mix_Pause(channel);
}

void resume(int channel)
{
	Mix_Resume(channel);
}

void volume(int channel, int volume)
{
	Mix_Volume(channel, volume);
}

void channel_finished(void(*channel_finished)(int channel))
{
	Mix_ChannelFinished(channel_finished);
}

}