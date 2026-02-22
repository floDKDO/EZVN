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
	scroll_ = std::make_unique<ScrollableArea>("Resolutions", 200, 0, 1000, 720, renderer);
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

void HistoryMenu::add_dialogue_to_history(std::string_view character_name, std::string_view dialogue, Color namebox_text_color, sdl::Renderer& renderer)
{
	//std::cout << "Character: " << character_name << ", dialogue: " << dialogue << ", color: r:" << int(namebox_text_color.r_) << ", g:" << int(namebox_text_color.g_) << ", b:" << int(namebox_text_color.b_) << ", a: " << int(namebox_text_color.a_) << std::endl;
	/*texts_.push_back({
		Text(character_name, namebox_text_color, constants::namebox_font_, 30, 0, 0, renderer),
		Text(dialogue, constants::textbox_text_color_, constants::textbox_font_, 30, 0, 0, renderer)
	});*/

	static int y_spacing_between_each_text = 0;

	//std::cout << "Color: " << int(namebox_text_color.r_) << ", " << int(namebox_text_color.g_) << ", " << int(namebox_text_color.b_) << ", " << int(namebox_text_color.a_) << std::endl;

	scroll_ptr_->add_text({
		std::make_unique<Text>(character_name, namebox_text_color, constants::namebox_font_, 50, 50, y_spacing_between_each_text, renderer),
		std::make_unique<Text>(dialogue, constants::textbox_text_color_, constants::textbox_font_, 50, 200, y_spacing_between_each_text, renderer)
	});

	y_spacing_between_each_text += 80; //TODO : hardcodé
}

void HistoryMenu::previous_menu_function(Ui* ui)
{
	std::cout << "Pressed => go back to previous menu!" << std::endl;
	game_.request_pop_state();
}