#include "main_menu.h"
#include "GUI/text_button.h"
#include "game.h"

#include <iostream>

MainMenu::MainMenu(Game& game, std::string_view background_path, sdl::Renderer& renderer)
	: GameState(game), background_(background_path, 0, 0, renderer)
{
	build_ui_elements(renderer);
}

void MainMenu::build_ui_elements(sdl::Renderer& renderer)
{
	//ici, std::placeholders::_1 est nécessaire car l'appel à la fonction de callback (ce qui est retourné par std::bind) est de la forme : f(this) => on spécifie l'argument lors de l'appel au callable et pas directement sa valeur dans std::bind
	ui_manager_.add_element(std::make_unique<TextButton>("Play", 600, 200, renderer, std::bind(&MainMenu::play_function, this, std::placeholders::_1)));
	ui_manager_.add_element(std::make_unique<TextButton>("Settings", 600, 350, renderer, std::bind(&MainMenu::settings_function, this, std::placeholders::_1)));
	//ui_elements_.push_back(std::make_unique<TextButton>("Quit", 600, 500, renderer, "Are you sure you want to quit?", std::bind(&MainMenu::confirmationpopup_quit_function, this, std::placeholders::_1)));
	ui_manager_.add_element(std::make_unique<TextButton>("Quit", 600, 500, renderer, std::bind(&MainMenu::confirmationpopup_quit_function, this, std::placeholders::_1)));

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
	std::cout << "Pressed Yes!" << std::endl;
	game_.quit_game();
}