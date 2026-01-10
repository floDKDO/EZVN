#pragma once

#include "RAII_SDL2/music.h"

struct Music
{
	sdl::Music music_;
	float volume_multiplier_;
};