#pragma once

#include <SDL2/SDL.h>

class Duration
{
	public:
		enum class Kind
		{
			LINEAR,
			EASE,
			EASEIN,
			EASEOUT
		};

		Duration();
		Duration(Uint64 duration, Kind kind);

		Uint64 duration_;
		Kind kind_;

	private:
};

