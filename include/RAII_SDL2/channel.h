#pragma once

#include "RAII_SDL2/chunk.h"

namespace sdl::channel
{

bool playing(int channel);
void play_channel(sdl::Chunk& chunk, int channel, bool loop, int volume = MIX_MAX_VOLUME); //not the same as Mix_PlayChannel() => loop is a bool (true <=> -1, false <=> 0). It also has a additional parameter (volume)
void fade_in(sdl::Chunk& chunk, int channel, bool loop, int ms, int volume = MIX_MAX_VOLUME); //not the same as Mix_FadeInChannel() => loop is a bool (true <=> -1, false <=> 0). It also has a additional parameter (volume)
void fade_out(int which, int ms);
void halt_channel(int channel);
void pause(int channel);
void resume(int channel);
void volume(int channel, int volume);
void channel_finished(void(*channel_finished)(int channel));

}