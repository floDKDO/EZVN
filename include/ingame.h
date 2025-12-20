#pragma once

#include "gamestate.h"
#include "textbox.h"
#include "character.h"
#include "background.h"
#include "GUI/texttoggle.h"
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

		TextToggle* get_texttoggle(const std::string_view text);

		void add_character(const std::string_view name, const std::string_view character_path, sdl::Renderer& renderer);
		void add_character(const std::string_view name, const std::string_view character_path, sdl::Renderer& renderer, const std::string_view textbox_path, const std::string_view namebox_path="");
		Character* get_character(const std::string_view name);

		void insert_dialogue(const std::string_view character_name, const std::string_view dialogue);
		void insert_background(const std::string_view background_path);
		void insert_background(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a);
		void insert_character(const std::string_view character_name, const TransformName transform_name, const int zorder);
		void insert_character(const std::string_view character_name, const TransformName transform_name);

		void change_background(const Background& b);

		void show_next_dialogue(); 

		enum class WhichDialogue
		{
			next,
			previous
		};
		void show_dialogue_mouse_wheel(WhichDialogue which_dialogue);

		void auto_function(Ui* ui);
		void settings_function(Ui* ui);
		void temp_function(Ui* ui);

		std::vector<std::unique_ptr<Character>> characters_; //unique_ptr pour que stable_sort fonctionne

		//TODO : obligé d'utiliser Character* ??
		std::map<unsigned int, std::tuple<Character*, const TransformName, int>> characters_transforms_; 
		std::map<unsigned int, std::pair<const std::string_view, Character*>> dialogues_;
		std::map<unsigned int, Background> backgrounds_;

		//TODO : utilité de mutable ??
		mutable unsigned int unique_id_;
		unsigned int current_unique_id_;

		unsigned int current_unique_id_when_previous_;
		bool is_current_unique_id_saved_;

		Uint64 last_time_;

		Textbox textbox_; //TODO : remettre private

	private:
		Background background_; 
		bool hide_ui_textbox_;
		sdl::Renderer &renderer_;
};

