#include "SDL/animation.h"

namespace sdl
{

Animation::Animation(const std::string file) //IMG_LoadAnimation
{
	this->animation = IMG_LoadAnimation(file.c_str());
}

Animation::Animation(const Animation& animation)
{
	//TODO!
}

Animation& Animation::operator=(const Animation& animation)
{
	//TODO!
	return *this;
}

Animation::~Animation() //IMG_FreeAnimation
{
	IMG_FreeAnimation(this->animation);
}

IMG_Animation* Animation::Get() const
{
	return this->animation;
}

}