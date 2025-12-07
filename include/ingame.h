#pragma once

#include "gamestate.h"
#include "textbox.h"
#include "character.h"
#include <vector>
#include <memory>
#include <map>

//TODO : ne pas utiliser .get() avec un unique_ptr ??

class InGame : public GameState
{
	public:
		InGame(Game& game, sdl::Renderer& renderer);

		void build_ui_elements(sdl::Renderer& renderer) override;
		void handle_events(const SDL_Event& e) override;
		void draw(sdl::Renderer& renderer) override;
		void update() override;

		void add_character(const std::string name, const std::string character_path, sdl::Renderer& renderer);
		Character* get_character(const std::string name);

		void temp_function(Ui* ui);

		std::vector<std::unique_ptr<Character>> characters_;

		//TODO : enregistrer la ligne de la demande
		std::map<unsigned int, std::pair<Character*, const TransformName>> characters_transforms_; 
		std::map<unsigned int, std::pair<const std::string, Character*>> dialogues_;

		unsigned int counter_; //TODO : renommer
		unsigned int current_line_; //TODO : remplacer par le numéro de ligne courant

		Textbox textbox_; //TODO : remettre private

	private:
		
		Image background_;
		bool hide_ui_textbox_;
};

