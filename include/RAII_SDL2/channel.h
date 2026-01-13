#pragma once

#include "RAII_SDL2/chunk.h"

namespace sdl::channel
{

bool playing(int channel);
void play_channel(sdl::Chunk& chunk, int channel, const bool loop, const int volume = MIX_MAX_VOLUME); //not the same as Mix_PlayChannel() => loop is a bool (true <=> -1, false <=> 0). It also has a additional parameter (volume)
void fade_in(sdl::Chunk& chunk, const int channel, const bool loop, const int ms, const int volume = MIX_MAX_VOLUME); //not the same as Mix_FadeInChannel() => loop is a bool (true <=> -1, false <=> 0). It also has a additional parameter (volume)
void fade_out(const int which, const int ms);
void halt_channel(int channel);
void pause(const int channel);
void resume(const int channel);
void volume(int channel, const int volume);

}