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
	scroll_ = std::make_unique<ScrollableArea>("Resolutions", 370, 0, 900, 720, renderer); //dialogue, x = 500, character_name = 380
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

void HistoryMenu::add_dialogue_to_history(std::string_view character_name, std::string_view dialogue, Color namebox_text_color, int textbox_width, sdl::Renderer& renderer)
{
	//std::cout << "Character: " << character_name << ", dialogue: " << dialogue << ", color: r:" << int(namebox_text_color.r_) << ", g:" << int(namebox_text_color.g_) << ", b:" << int(namebox_text_color.b_) << ", a: " << int(namebox_text_color.a_) << std::endl;
	/*texts_.push_back({
		Text(character_name, namebox_text_color, constants::namebox_font_, 30, 0, 0, renderer),
		Text(dialogue, constants::textbox_text_color_, constants::textbox_font_, 30, 0, 0, renderer)
	});*/

	static int y_spacing_between_each_text = 0;

	//std::cout << "Color: " << int(namebox_text_color.r_) << ", " << int(namebox_text_color.g_) << ", " << int(namebox_text_color.b_) << ", " << int(namebox_text_color.a_) << std::endl;

	scroll_ptr_->add_text({
		std::make_unique<Text>(character_name, namebox_text_color, constants::namebox_font_, constants::namebox_text_size_, 380, y_spacing_between_each_text, renderer, false, textbox_width - (constants::textbox_text_x_delta_ * 2)),
		std::make_unique<Text>(dialogue, constants::textbox_text_color_, constants::textbox_font_, constants::textbox_text_size_, 500, y_spacing_between_each_text, renderer, false, textbox_width - (constants::textbox_text_x_delta_ * 2))
	});

	y_spacing_between_each_text += 80; //TODO : hardcodé et ne marche pas => il faudrait que cette valeur dépende de la taille de chaque dialogue
}

void HistoryMenu::previous_menu_function(Ui* ui)
{
	std::cout << "Pressed => go back to previous menu!" << std::endl;
	game_.request_pop_state();
}