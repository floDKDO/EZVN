#include "mainmenu.h"
#include "GUI/textbutton.h"
//#include "GUI/buttongroup.h"
#include "game.h"

#include <iostream>

MainMenu::MainMenu(Game& game, const std::string_view background_path, sdl::Renderer& renderer)
	: GameState(game), background_(background_path, 0, 0, renderer)
{
	build_ui_elements(renderer);
}

void MainMenu::build_ui_elements(sdl::Renderer& renderer)
{
	ui_elements_.reserve(10);

	//ici, std::placeholders::_1 est nécessaire car l'appel à la fonction de callback (ce qui est retourné par std::bind) est de la forme : f(this) => on spécifie l'argument lors de l'appel au callable et pas directement sa valeur dans std::bind
	ui_elements_.push_back(std::make_unique<TextButton>("Play", 600, 200, renderer, std::bind(&MainMenu::play_function, this, std::placeholders::_1)));
	ui_elements_.push_back(std::make_unique<TextButton>("Settings", 600, 350, renderer, std::bind(&MainMenu::settings_function, this, std::placeholders::_1)));
	ui_elements_.push_back(std::make_unique<TextButton>("Quit", 600, 500, renderer, "Are you sure you would like to close the game?", std::bind(&MainMenu::confirmationpopup_quit_function, this, std::placeholders::_1)));
	//ui_elements_.push_back(std::make_unique<ButtonGroup<2>>(std::vector<std::string_view>{"Windowed", "Fullscreen"}, 50, renderer));
}

void MainMenu::handle_events(const SDL_Event& e)
{
	GameState::handle_events(e);
}

void MainMenu::draw(sdl::Renderer& renderer)
{
	background_.draw(renderer);
	GameState::draw(renderer);
}

void MainMenu::update()
{
	GameState::update();
}

void MainMenu::play_function(Ui* ui)
{
	(void)ui;
	std::cout << "Clicked Play!" << std::endl;
	std::cout << game_.in_game_->ui_elements_.size() << std::endl;
	game_.push_state(game_.in_game_.get()); //TODO : simplifier avec un truc plus générique (ex : game_.change_state("Ingame"))
}

void MainMenu::settings_function(Ui* ui)
{
	(void)ui;
	std::cout << "Clicked Settings!" << std::endl;
	game_.push_state(game_.settings_menu_.get());
}

void MainMenu::confirmationpopup_quit_function(Ui* ui)
{
	(void)ui;
	std::cout << "Clicked Yes!" << std::endl;
	game_.game_running_ = false;
}