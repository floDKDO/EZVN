#include "GameStates/save_load_menu.h"
#include "GameStates/in_game.h"
#include "game.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <ctime>
#include <iostream>

using json = nlohmann::json;


SaveLoadMenu::SaveLoadMenu(Game& game, sdl::Renderer& renderer)
	: GameState(game, renderer), background_(constants::default_menu_background_, 0, 0, renderer), renderer_(renderer)
{
	build_ui_elements(renderer);
}

void SaveLoadMenu::build_ui_elements(sdl::Renderer& renderer)
{
	add_ui_element(std::make_unique<TextButton>("Return", 100, 550, renderer, std::bind(&SaveLoadMenu::previous_menu_function, this, std::placeholders::_1)));
	button_ptr_ = dynamic_cast<Button*>(add_ui_element(std::make_unique<Button>("resources/img/gui/empty_save_normal.png", "resources/img/gui/empty_save_selected.png", "resources/img/gui/empty_save_selected.png", "empty slot", 500, 200, renderer, std::bind(&SaveLoadMenu::save_function, this, std::placeholders::_1), TextPosition::UNDER))); //TODO : hardcodé
	ui_manager_.set_elements();
}

void SaveLoadMenu::draw(sdl::Renderer& renderer)
{
	background_.draw(renderer);
	GameState::draw(renderer);
}

void SaveLoadMenu::update()
{
	background_.update();
	GameState::update();
}

void SaveLoadMenu::previous_menu_function([[maybe_unused]] Ui* ui)
{
	std::cout << "Pressed => go back to previous menu!" << std::endl;
	game_.request_pop_state();
}

void SaveLoadMenu::save_function([[maybe_unused]] Ui* ui)
{
	std::string save_filename = dynamic_cast<InGame*>(game_.get_state(constants::ingame_unique_id_))->script_runner_->save_filename_;
	std::ifstream input_file(save_filename);
	json data;

	if(input_file.is_open())
	{
		data = json::parse(input_file, nullptr, false);

		if(data.is_discarded())
		{
			data = json::object();
		}
	}
	input_file.close();

	/* Structure d'une sauvegarde :
	* saveX: { //avec X l'emplacement de la sauvegarde
	*   "current_script_index_": valeur,
	*   "screenshot": chemin du screenshot,
	*   "time": date et heure
	* }
	*/

	IMG_SavePNG(game_.screenshot_, "temp.png"); //TODO : hardcodé
	size_t current_script_index = dynamic_cast<InGame*>(game_.get_state(constants::ingame_unique_id_))->script_runner_->current_script_index_;
	std::time_t date = std::time(nullptr);
	std::string time = std::string(std::ctime(&date));

	data["save"] = {{"current_script_index", current_script_index}, {"screenshot", "temp.png"}, {"time", time}};

	std::ofstream output_file(save_filename);
	output_file << data.dump(4);
	output_file.close();

	button_ptr_->normal_.new_image("temp.png", renderer_); //TODO : hardcodé
	button_ptr_->selected_.new_image("temp.png", renderer_); //TODO : hardcodé
	button_ptr_->pressed_.new_image("temp.png", renderer_); //TODO : hardcodé
	button_ptr_->text_.text_ = time; //TODO : hardcodé
}