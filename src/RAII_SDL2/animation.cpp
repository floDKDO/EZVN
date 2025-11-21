#include "RAII_SDL2/animation.h"

namespace sdl
{

Animation::Animation(const std::string file) //IMG_LoadAnimation
{
	animation_ = IMG_LoadAnimation(file.c_str()); //=> GIF ou WEBP
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