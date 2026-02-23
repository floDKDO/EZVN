#include "Managers/choice_menu_manager.h"
#include "GUI/button.h"

#include <iostream>

ChoiceMenuManager::ChoiceMenuManager(sdl::Renderer& renderer, Game& game)
	: choice_made_(false), ui_group_(nullptr), ui_manager_(game.audio_manager_), renderer_(renderer)
{
	build_ui_elements(renderer);
}

void ChoiceMenuManager::build_ui_elements(sdl::Renderer& renderer)
{
	std::unique_ptr<UiGroup> ui_group = std::make_unique<UiGroup>(550, 80);
	ui_group_ = ui_group.get();
	ui_manager_.add_element(std::move(ui_group));
	ui_manager_.set_elements();
}

void ChoiceMenuManager::handle_events(const SDL_Event& e)
{
	if(ui_group_->ui_elements_.size() > 0 && !choice_made_)
	{
		ui_manager_.handle_events(e);
	}
}

void ChoiceMenuManager::draw(sdl::Renderer& renderer)
{
	if(ui_group_ != nullptr && !choice_made_)
	{
		ui_group_->draw(renderer);
	}
}

void ChoiceMenuManager::update(const Script::InfoChoiceMenu& info_choice_menu)
{
	//if(ui_group_ == nullptr)
	if(ui_group_->ui_elements_.size() == 0)
	{
		choice_made_ = false;
		for(int i = 0; i < info_choice_menu.texts_.size(); ++i)
		{
			ui_group_->add_ui_element(std::make_unique<Button>(info_choice_menu.texts_[i], 0, 0, renderer_, [this, info_choice_menu, i](Ui* ui){std::cout << info_choice_menu.texts_[i] << std::endl; choice_made_ = true; })); //TODO : hardcodé
		}
		ui_manager_.set_elements();
	}
	else
	{
		//ui_group_.reset();
		if(!choice_made_)
		{
			ui_group_->update();
		}
	}
}

void ChoiceMenuManager::reset()
{

}