#include "GameStates/main_menu.h"
#include "GUI/text_button.h"
#include "game.h"

#include <iostream>

MainMenu::MainMenu(Game& game, sdl::Renderer& renderer)
	: GameState(game, renderer), background_(constants::default_menu_background_, 0, 0, renderer)
{
	build_ui_elements(renderer);
}

void MainMenu::build_ui_elements(sdl::Renderer& renderer)
{
	//ici, std::placeholders::_1 est nécessaire car l'appel à la fonction de callback (ce qui est retourné par std::bind) est de la forme : f(this) => on spécifie l'argument lors de l'appel au callable et pas directement sa valeur dans std::bind
	add_ui_element(std::make_unique<TextButton>("Play", 600, 200, renderer, std::bind(&MainMenu::play_function, this, std::placeholders::_1)));
	add_ui_element(std::make_unique<TextButton>("Settings", 600, 350, renderer, std::bind(&MainMenu::settings_function, this, std::placeholders::_1)));
	add_ui_element(std::make_unique<TextButton>("Quit", 600, 500, renderer, std::bind(&MainMenu::confirmationpopup_quit_function, this, std::placeholders::_1)));

	ui_manager_.set_elements();
}

void MainMenu::draw(sdl::Renderer& renderer)
{
	background_.draw(renderer);
	GameState::draw(renderer);
}

void MainMenu::update()
{
	background_.update();
	GameState::update();
}

void MainMenu::temp_function([[maybe_unused]] Ui* ui)
{
	std::cout << "Pressed!\n";
}

void MainMenu::play_function([[maybe_unused]] Ui* ui)
{
	std::cout << "Pressed Play!" << std::endl;
	game_.request_push_state(constants::ingame_unique_id_);
}

void MainMenu::settings_function([[maybe_unused]] Ui* ui)
{
	std::cout << "Pressed Settings!" << std::endl;
	game_.request_push_state(constants::settings_menu_unique_id_);
}

void MainMenu::confirmationpopup_quit_function([[maybe_unused]] Ui* ui)
{
	std::cout << "Pressed Quit!" << std::endl;
	ui_manager_.show_pop_up(constants::confirmationpopup_message_, 
		[this]([[maybe_unused]] Ui* ui){
		std::cout << "Pressed Yes!" << std::endl; 
		game_.quit_game(); 
	});
}