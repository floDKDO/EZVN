#pragma once

#include <SDL2/SDL_image.h>
#include <string>

namespace sdl
{

class Animation
{
	public:
	explicit Animation(const std::string file); //IMG_LoadAnimation
	Animation(const Animation& animation) = delete;
	Animation& operator=(const Animation& animation) = delete;
	~Animation(); //IMG_FreeAnimation

	IMG_Animation* Get() const;

	protected:

	private:
	IMG_Animation* animation;
};
}
