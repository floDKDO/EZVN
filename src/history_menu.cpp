#include "history_menu.h"
#include "game.h"
#include "GUI/text_button.h"

#include <iostream>

HistoryMenu::HistoryMenu(Game& game, sdl::Renderer& renderer)
	: GameState(game), background_(constants::default_menu_background_, 0, 0, renderer)
{
	create_history_scroll_area(renderer);
	build_ui_elements(renderer);
}

void HistoryMenu::create_history_scroll_area(sdl::Renderer& renderer)
{
	scroll_ = std::make_unique<ScrollableArea>("Resolutions", 950, 170, 200, 500, renderer);
	scroll_ptr_ = scroll_.get();
}

void HistoryMenu::build_ui_elements(sdl::Renderer& renderer)
{
	ui_manager_.add_element(std::make_unique<TextButton>("Return", 100, 550, renderer, std::bind(&HistoryMenu::previous_menu_function, this, std::placeholders::_1)));
	ui_manager_.add_element(std::move(scroll_));
	ui_manager_.set_elements();
}

void HistoryMenu::draw(sdl::Renderer& renderer)
{
	background_.draw(renderer);
	GameState::draw(renderer);
}

void HistoryMenu::update()
{
	background_.update();
	GameState::update();
}

void HistoryMenu::add_dialogue_to_history(std::string_view character_name, std::string_view dialogue, Color namebox_text_color)
{
	//std::cout << "Character: " << character_name << ", dialogue: " << dialogue << ", color: r:" << int(namebox_text_color.r_) << ", g:" << int(namebox_text_color.g_) << ", b:" << int(namebox_text_color.b_) << ", a: " << int(namebox_text_color.a_) << std::endl;
	texts_.push_back({std::string(character_name), std::string(dialogue)});

	for(auto& p : texts_)
	{
		std::cout << p.first << ", " << p.second << std::endl;
	}
}

void HistoryMenu::previous_menu_function(Ui* ui)
{
	std::cout << "Pressed => go back to previous menu!" << std::endl;
	game_.request_pop_state();
}