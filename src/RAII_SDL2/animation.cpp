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

Animation::Animation(Animation&& animation)
	: animation_(animation.animation_)
{
	animation.animation_ = nullptr;
}

Animation& Animation::operator=(Animation&& animation)
{
	if(this == &animation)
	{
		return *this;
	}

	if(animation_ != nullptr)
	{
		IMG_FreeAnimation(animation_);
	}

	animation_ = animation.animation_;
	animation.animation_ = nullptr;
	return *this;
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