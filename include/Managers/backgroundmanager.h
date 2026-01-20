#pragma once

#include "script.h"

class BackgroundManager
{
	public:
		explicit BackgroundManager(sdl::Renderer& renderer);

		void draw(sdl::Renderer& renderer);
		void update(const Script::InfoBackground& info_background);

	private:
		Background background_;
		sdl::Renderer& renderer_;

};

