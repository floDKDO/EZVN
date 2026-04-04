#include "GameStates/in_game.h"
#include "game.h"

#include <iostream>

//Info : le UiManager est passé au ScriptRunner => c'est lui qui appelle ses méthodes handle_events(), draw() et update()

InGame::InGame(Game& game, sdl::Renderer& renderer)
	: GameState(game, renderer), renderer_(renderer)
{
	build_ui_elements(renderer);
	script_runner_ = std::make_unique<ScriptRunner>(ui_on_textbox_, ui_manager_, game, renderer);
}

void InGame::build_ui_elements(sdl::Renderer& renderer)
{
	std::unique_ptr<UiGroup> ui_group = std::make_unique<UiGroup>("", 0, 0, renderer);
	ui_on_textbox_.history_button_ = dynamic_cast<TextButton*>(ui_group->add_ui_element(std::make_unique<TextButton>("History", 0, 0, renderer, nullptr, TextButton::Kind::ON_TEXTBOX))); //TODO : pas ouf => à terme, utiliser des forwarding references pour que le bon type soit retourné //pas de fonction de callback
	ui_on_textbox_.skip_toggle_ = dynamic_cast<TextToggle*>(ui_group->add_ui_element(std::make_unique<TextToggle>("Skip", 0, 0, false, renderer, nullptr, TextToggle::Kind::ON_TEXTBOX))); //TODO : pas ouf => à terme, utiliser des forwarding references pour que le bon type soit retourné
	ui_on_textbox_.auto_toggle_ = dynamic_cast<TextToggle*>(ui_group->add_ui_element(std::make_unique<TextToggle>("Auto", 0, 0, false, renderer, nullptr, TextToggle::Kind::ON_TEXTBOX))); //TODO : pas ouf => à terme, utiliser des forwarding references pour que le bon type soit retourné
	ui_on_textbox_.save_button_ = dynamic_cast<TextButton*>(ui_group->add_ui_element(std::make_unique<TextButton>("Save", 0, 0, renderer, nullptr, TextButton::Kind::ON_TEXTBOX))); //TODO : pas ouf => à terme, utiliser des forwarding references pour que le bon type soit retourné
	ui_on_textbox_.load_button_ = dynamic_cast<TextButton*>(ui_group->add_ui_element(std::make_unique<TextButton>("Load", 0, 0, renderer, nullptr, TextButton::Kind::ON_TEXTBOX))); //TODO : pas ouf => à terme, utiliser des forwarding references pour que le bon type soit retourné
	ui_on_textbox_.settings_button_ = dynamic_cast<TextButton*>(ui_group->add_ui_element(std::make_unique<TextButton>("Settings", 0, 0, renderer, nullptr, TextButton::Kind::ON_TEXTBOX))); //TODO : pas ouf => à terme, utiliser des forwarding references pour que le bon type soit retourné
	add_ui_element(std::move(ui_group)); 

	ui_manager_.set_elements();
}

void InGame::handle_events(const SDL_Event& e)
{
	script_runner_->handle_events(e);
}

void InGame::draw(sdl::Renderer& renderer)
{
	script_runner_->draw(renderer);
}

void InGame::update()
{
	script_runner_->update();
}