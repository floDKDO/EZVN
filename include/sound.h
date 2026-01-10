#pragma once

#include "RAII_SDL2/chunk.h"

struct Sound
{
	sdl::Chunk sound_;
	float volume_multiplier_;
};
