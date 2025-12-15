#pragma once

#include "gamestate.h"
#include "textbox.h"
#include "character.h"
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
		Character* get_character(const std::string_view name);

		void insert_dialogue(const std::string_view character_name, const std::string_view dialogue);
		void insert_background(const std::string_view background_path);
		void insert_character(const std::string_view character_name, const TransformName transform_name, const int zorder);

		void change_background(const std::string_view background_path);

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

		std::map<unsigned int, std::tuple<Character*, const TransformName, int>> characters_transforms_; 
		std::map<unsigned int, std::pair<const std::string_view, Character*>> dialogues_;
		std::map<unsigned int, const std::string_view> backgrounds_;

		mutable unsigned int unique_id_;
		unsigned int current_unique_id_;

		unsigned int current_unique_id_when_previous_;
		bool is_current_unique_id_saved_;

		Uint64 last_time_;

		Textbox textbox_; //TODO : remettre private

	private:
		std::unique_ptr<Image> background_; //TODO : utiliser un pointeur pour que l'image puisse être vide ??
		bool hide_ui_textbox_;
		sdl::Renderer &renderer_;
};

