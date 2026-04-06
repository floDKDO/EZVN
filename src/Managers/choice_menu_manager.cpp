#include "Managers/choice_menu_manager.h"
#include "GUI/button.h"

#include <iostream>

ChoiceMenuManager::ChoiceMenuManager(UiManager& ui_manager, sdl::Renderer& renderer, Game& game)
	: choice_made_(false), is_visible_(false), ui_group_(nullptr), ui_manager_(ui_manager), renderer_(renderer)
{
	ui_group_ = std::make_unique<UiGroup>(0, constants::choice_menu_y_delta_, Layout::VERTICAL);
	ui_manager_.register_element(ui_group_.get());
	//ui_manager_.set_elements();
}

void ChoiceMenuManager::hide()
{
	is_visible_ = false;
	ui_group_->is_visible_ = false;
	for(const std::unique_ptr<UiWidget>& ui : ui_group_->ui_elements_)
	{
		ui->is_visible_ = false;
	}
	choice_made_ = false;
	clear_before_update();
}

void ChoiceMenuManager::clear_before_update()
{
	all_after_choice_dialogues_.clear();
	ui_group_->clear();
	ui_manager_.update_navigation_list();
}

void ChoiceMenuManager::draw(sdl::Renderer& renderer)
{
	if(is_visible_ && ui_group_->is_visible_)
	{
		ui_group_->draw(renderer);
	}
}

void ChoiceMenuManager::update(const Script::InfoChoiceMenu& info_choice_menu)
{
	clear_before_update(); 

	for(int i = 0; i < info_choice_menu.texts_.size(); ++i)
	{
		ui_group_->add_ui_element(std::make_unique<Button>(constants::choice_button_normal_, constants::choice_button_selected_, constants::choice_button_pressed_, info_choice_menu.texts_[i].first, 0, 0, renderer_,
			[this, info_choice_menu, i]([[maybe_unused]] Ui* ui)
		{
			std::cout << "You chose: " << info_choice_menu.texts_[i].first << std::endl;
			after_choice_dialogue_chosen_ = {info_choice_menu.texts_[i].second.character_variable_, info_choice_menu.texts_[i].second.dialogue_};
			choice_made_ = true;
			is_visible_ = false;
		}
		));
		all_after_choice_dialogues_.push_back({info_choice_menu.texts_[i].second.character_variable_, info_choice_menu.texts_[i].second.dialogue_});
	}
	ui_group_->set_center();

	choice_made_ = false;
	is_visible_ = true;
	ui_group_->is_visible_ = true;

	ui_manager_.update_navigation_list();
}

void ChoiceMenuManager::update_buttons()
{
	if(is_visible_ && ui_group_->is_visible_)
	{
		ui_group_->update();
	}
}

void ChoiceMenuManager::reset()
{
	clear_before_update();
	//all_after_choice_dialogues_.clear();
	after_choice_dialogue_chosen_.character_variable_.clear();
	after_choice_dialogue_chosen_.dialogue_.clear();
	choice_made_ = false;
	is_visible_ = false;
}