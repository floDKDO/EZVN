#pragma once

#include "gamestate.h"
#include "textbox.h"
#include "character.h"
#include <vector>
#include <memory>
#include <map>

class InGame : public GameState
{
	public:
		InGame(Game& game, sdl::Renderer& renderer);

		void build_ui_elements(sdl::Renderer& renderer) override;
		void handle_events(const SDL_Event& e) override;
		void draw(sdl::Renderer& renderer) override;
		void update() override;

		void add_character(const std::string_view name, const std::string_view character_path, sdl::Renderer& renderer);
		Character* get_character(const std::string_view name);

		void change_background(const std::string_view background_path, sdl::Renderer& renderer);

		void temp_function(Ui* ui);

		std::vector<std::unique_ptr<Character>> characters_;

		//TODO : enregistrer la ligne de la demande
		std::map<unsigned int, std::pair<Character*, const TransformName>> characters_transforms_; 
		std::map<unsigned int, std::pair<const std::string_view, Character*>> dialogues_;
		std::map<unsigned int, const std::string_view> backgrounds_;

		unsigned int counter_; //TODO : renommer
		unsigned int current_line_; //TODO : remplacer par le numéro de ligne courant

		Textbox textbox_; //TODO : remettre private

	private:
		std::unique_ptr<Image> background_; //TODO : utiliser un pointeur pour que l'image puisse être vide ??
		bool hide_ui_textbox_;
};

