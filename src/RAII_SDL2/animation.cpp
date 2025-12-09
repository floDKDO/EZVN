#include "RAII_SDL2/animation.h"

namespace sdl
{

Animation::Animation(const std::string_view file) //IMG_LoadAnimation
{
	if((animation_ = IMG_LoadAnimation(file.data())) == nullptr) //=> GIF ou WEBP
	{
		SDL_Log("(IMG_LoadAnimation) %s\n", IMG_GetError());
	}
}

Animation::~Animation() //IMG_FreeAnimation
{
	IMG_FreeAnimation(animation_);
}

IMG_Animation* Animation::fetch() const
{
	return animation_;
}

}