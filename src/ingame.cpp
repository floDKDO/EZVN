#include "ingame.h"
#include "constants.h"
#include "game.h"

#include <iostream>

//TODO : le code des dialogues devra être modifié quand il y aura l'ajout de pauses, animations d'images, choice menus etc.

InGame::InGame(Game& game, sdl::Renderer& renderer)
	: GameState(game), script_runner_(game, renderer), renderer_(renderer)
{}

void InGame::handle_events(const SDL_Event& e)
{
	script_runner_.handle_events(e);
}

void InGame::draw(sdl::Renderer& renderer)
{
	script_runner_.draw(renderer);
}

void InGame::update_characters_dialogue(Script::InfoTextbox& info_textbox)
{
	//if(which_dialogue_from_where_.which_dialogue_ == ScriptRunner::Where::none) //TODO : ajouter && !textbox_.is_first_dialogue_ ??
	//{
	//	return;
	//}

	//TODO : attention : conflit avec l'autofocus
	/*for(auto& [key_character_variable, value_character] : character_manager_.active_characters_)
	{
		value_character.properties_.is_speaking_ = false;
	}*/

	//std::cout << "PERSO: " << info_textbox.character_variable_ << ", texte: " << info_textbox.t_ << std::endl;

	//Character& character = character_manager_.active_characters_.at(info_textbox.character_variable_);
	/*character.properties_.is_speaking_ = true;*/ //TODO : attention : conflit avec l'autofocus
}

void InGame::update()
{
	script_runner_.update();
}