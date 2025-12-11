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

		void change_background(const std::string_view background_path);

		void temp_function(Ui* ui);

		std::vector<std::unique_ptr<Character>> characters_; //unique_ptr pour que stable_sort fonctionne

		std::map<unsigned int, std::tuple<Character*, const TransformName, int>> characters_transforms_; 
		std::map<unsigned int, std::pair<const std::string_view, Character*>> dialogues_;
		std::map<unsigned int, const std::string_view> backgrounds_;

		unsigned int dialogue_index_; 
		unsigned int current_line_; 

		Textbox textbox_; //TODO : remettre private

	private:
		std::unique_ptr<Image> background_; //TODO : utiliser un pointeur pour que l'image puisse être vide ??
		bool hide_ui_textbox_;
		sdl::Renderer &renderer_;
};

