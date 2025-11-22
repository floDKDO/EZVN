#include "RAII_SDL2/animation.h"

namespace sdl
{

Animation::Animation(const std::string file) //IMG_LoadAnimation
{
	if((animation_ = IMG_LoadAnimation(file.c_str())) == nullptr) //=> GIF ou WEBP
	{
		SDL_Log("(IMG_LoadAnimation) %s\n", IMG_GetError());
	}
}

Animation::~Animation() //IMG_FreeAnimation
{
	IMG_FreeAnimation(animation_);
}

IMG_Animation* Animation::Get() const
{
	return animation_;
}

}