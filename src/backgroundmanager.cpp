#include "backgroundmanager.h"


BackgroundManager::BackgroundManager(unsigned int& unique_id, unsigned int& current_unique_id, sdl::Renderer& renderer)
	: background_(0, 0, 0, 255), unique_id_(unique_id), current_unique_id_(current_unique_id), renderer_(renderer)
{}

void BackgroundManager::insert_background(const std::string_view background_path)
{
	backgrounds_.insert({unique_id_, Background(background_path, renderer_)});
	unique_id_ += 1;
}

void BackgroundManager::insert_background(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a)
{
	backgrounds_.insert({unique_id_, Background(r, g, b, a)});
	unique_id_ += 1;
}

void BackgroundManager::change_background(const Background& b)
{
	if(b.image_ != nullptr)
	{
		if(background_.image_ != nullptr)
		{
			background_.image_->init_image(b.image_->path_, 0, 0, renderer_);
		}
		else
		{
			background_ = Background(b.image_->path_, renderer_);
		}
	}
	else
	{
		background_.image_.reset();
		background_.color_ = {b.color_.r, b.color_.g, b.color_.b, b.color_.a};
	}
}

void BackgroundManager::draw()
{
	background_.draw(renderer_);
}

void BackgroundManager::update()
{
	for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(backgrounds_.count(i))
		{
			change_background(backgrounds_.at(i));
			break;
		}
	}
}