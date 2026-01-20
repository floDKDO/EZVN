#include "Managers/backgroundmanager.h"

BackgroundManager::BackgroundManager(sdl::Renderer& renderer)
	: background_(Color::from_rgba8(0, 0, 0)), renderer_(renderer)
{}

void BackgroundManager::draw(sdl::Renderer& renderer)
{
	background_.draw(renderer);
}

void BackgroundManager::update(const Script::InfoBackground& info_background)
{
	//Backgrounds
	if(info_background.image_ != nullptr)
	{
		if(background_.image_ != nullptr)
		{
			background_.image_->init_image(info_background.image_->path_, 0, 0, renderer_);
		}
		else
		{
			background_ = Background(info_background.image_->path_, renderer_);
		}
	}
	else
	{
		background_.image_.reset();
		background_.color_ = info_background.color_;
	}
}