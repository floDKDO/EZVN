#include "RAII_SDL2/animation.h"
#include "RAII_SDL2/rwops.h"

namespace sdl
{

Animation::Animation(std::string_view file) //IMG_LoadAnimation
{
	sdl::RWops rwops(file, "rb");
	if(rwops.is_gif())
	{
		if((animation_ = IMG_LoadGIFAnimation_RW(rwops.fetch())) == nullptr)
		{
			SDL_Log("(IMG_LoadGIFAnimation_RW) %s\n", IMG_GetError());
		}
	}
	else if(rwops.is_webp())
	{
		if((animation_ = IMG_LoadWEBPAnimation_RW(rwops.fetch())) == nullptr)
		{
			SDL_Log("(IMG_LoadWEBPAnimation_RW) %s\n", IMG_GetError());
		}
	}
	else
	{
		SDL_Log("Not an animation!\n");
		animation_ = nullptr;
		//TODO : exception
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
	if(animation_ != nullptr)
	{
		IMG_FreeAnimation(animation_);
	}
}

IMG_Animation* Animation::fetch() const
{
	return animation_;
}

}