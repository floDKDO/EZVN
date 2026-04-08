#include "game.h"
#include "GameStates/in_game.h"
#include "GameStates/main_menu.h"
#include "GameStates/settings_menu.h"
#include "GameStates/history_menu.h"
#include "utils.h"
#include "RAII_SDL2/pref_path.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

Game::Game()
	: history_menu_ptr_(nullptr), sdl_(SDL_INIT_EVERYTHING), sdl_img_(IMG_INIT_PNG | IMG_INIT_JPG), sdl_mixer_(MIX_INIT_OGG | MIX_INIT_MP3), sdl_ttf_(),
	window_(constants::game_name_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, constants::window_width_, constants::window_height_, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI),
	audio_manager_(),
	renderer_(window_, -1, SDL_RENDERER_PRESENTVSYNC),
	game_controller_(),
	window_icon_(constants::window_icon_), 
	window_is_open_(true), script_(renderer_)
{
	create_persistent_variables_file();

	window_.set_icon(window_icon_);

	renderer_.set_logical_size(constants::window_width_, constants::window_height_);
	renderer_.set_draw_blend_mode(SDL_BLENDMODE_BLEND);
}

void Game::create_persistent_variables_file()
{
	sdl::PrefPath pref_path;
	persistent_variables_filename_ = pref_path.get_pref_path() + '\\' + std::string(constants::persistent_variables_file_);
	std::ofstream output_file(persistent_variables_filename_, std::ios_base::app);
	output_file.close();
}

void Game::init_game_states()
{
	game_states_map_.insert({constants::ingame_unique_id_, std::make_unique<InGame>(*this, renderer_)});
	game_states_map_.insert({constants::main_menu_unique_id_, std::make_unique<MainMenu>(*this, renderer_)}); 
	game_states_map_.insert({constants::settings_menu_unique_id_, std::make_unique<SettingsMenu>(*this, renderer_)}); 
	game_states_map_.insert({constants::history_menu_unique_id_, std::make_unique<HistoryMenu>(*this, renderer_)});
	push_state(game_states_map_.at(constants::main_menu_unique_id_).get());

	history_menu_ptr_ = dynamic_cast<HistoryMenu*>(get_state(constants::history_menu_unique_id_));
}

void Game::run()
{
	const Uint64 FPS = 60; //TODO : limitation arbitraire ? Je pourrais la retirer et faire des déplacements constants pour tous les FPS
	const Uint64 FRAME_TIME = Uint64((1.0f / FPS) * 1000.0f);

	Uint64 second = 0;
	Uint64 begin_current_frame = 0;
	Uint64 end_current_frame = 0;
	unsigned int frame_count = 0;

	//mettre ce code ici (utilité : script_information_ est initialisé dans le constructeur de ScriptRunner)
	init_game_states();

	//TODO : vérifier que ma boucle de jeu est bonne
	while(window_is_open_)
	{
		begin_current_frame = SDL_GetTicks64();
		frame_count += 1;

		handle_events();
		update();
		draw();

		if(SDL_GetTicks64() - second >= 1000)
		{
			std::string fps_count = ", FPS: " + std::to_string(frame_count);
			update_fps_count(fps_count);
			second = SDL_GetTicks64();
			frame_count = 0;
		}
		end_current_frame = SDL_GetTicks64();

		Uint64 elapsed = end_current_frame - begin_current_frame;
		Uint64 delay = FRAME_TIME - elapsed;

		if(FRAME_TIME > elapsed)
		{
			SDL_Delay(Uint32(delay)); 
		}
	}
}

GameState* Game::get_state(std::string_view unique_id)
{
	return game_states_map_.at(unique_id).get();
}

void Game::request_push_state(std::string_view unique_id) //pour ne pas que les fonctions de callback rendent invalide current_selected_ via un appel direct à push_state qui vide le vector navigation_list_ dans lequel current_selected_ prend sa valeur
{
	RequestedAction requested_action = {Action::PUSH, unique_id};
	requested_actions_.push(requested_action);
}

void Game::request_pop_state()
{
	RequestedAction requested_action = {Action::POP, ""};
	requested_actions_.push(requested_action);
}

void Game::quit_game()
{
	window_is_open_ = false;
}

void Game::handle_requests()
{
	while(!requested_actions_.empty())
	{
		RequestedAction top = requested_actions_.top();
		if(top.action_ == Action::PUSH)
		{
			push_state(game_states_map_.at(top.unique_id_).get());
		}
		else if(top.action_ == Action::POP)
		{
			pop_state();
		}
		requested_actions_.pop();
	}
}

void Game::push_state(GameState* state)
{
	game_states_stack_.push(state);
	state->on_show();
}

void Game::pop_state()
{
	game_states_stack_.top()->on_hide();
	game_states_stack_.pop();
}

GameState* Game::get_current_state() const
{
	return game_states_stack_.top();
}

InGame* Game::get_ingame_state() const
{
	return dynamic_cast<InGame*>(game_states_map_.at(constants::ingame_unique_id_).get());
}

void Game::handle_events()
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_WINDOWEVENT:
				if(e.window.event == SDL_WINDOWEVENT_CLOSE)
				{
					window_is_open_ = false;
				}
				break;

			case SDL_KEYDOWN:
				switch(e.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						window_is_open_ = false;
						break;

					default:
						break;
				}
				break;

			default:
				break;
		}
		get_current_state()->handle_events(e);
	}
}

void Game::draw()
{
	renderer_.set_draw_color(0, 0, 0, 255); //TODO : à la place, une fonction de callback qui est appelée quand la fenêtre est redimensionnée ??
	renderer_.clear();
	get_current_state()->draw(renderer_);
	renderer_.present();
}

void Game::update()
{
	handle_requests();
	get_current_state()->update();
}

void Game::update_fps_count(std::string_view fps) const
{
	window_.set_title(std::string(constants::game_name_) + std::string(fps));
}

void Game::create_composite_image(std::string_view character_variable, std::string_view composite_image_name, int w, int h, std::initializer_list<ImageInfo> images_info)
{
	script_.add_composite_image(character_variable, composite_image_name, w, h, images_info);
}

void Game::create_character(std::string_view character_variable, std::string_view character_name, Color namebox_text_color, std::string_view textbox_path, std::string_view namebox_path)
{
	script_.add_character(character_variable, character_name, namebox_text_color, textbox_path, namebox_path);
}

void Game::rename_character(std::string_view character_variable, std::string_view new_character_name)
{
	script_.rename_character(character_variable, new_character_name);
}

void Game::show_character(std::pair<std::string_view, std::string_view> character_info, std::string transform_name, unsigned int zorder)
{
	script_.show_character(character_info.first, character_info.second, transform_name, zorder);
}

void Game::show_character(std::string_view character_variable, std::string_view transform_name, unsigned int zorder)
{
	script_.show_character(character_variable, std::nullopt, transform_name, zorder);
}

void Game::show_character(std::pair<std::string_view, std::string_view> character_info, std::string transform_name)
{
	script_.show_character(character_info.first, character_info.second, transform_name, std::nullopt);
}

void Game::show_character(std::string_view character_variable, std::string_view transform_name)
{
	script_.show_character(character_variable, std::nullopt, transform_name, std::nullopt);
}

void Game::show_character(std::pair<std::string_view, std::string_view> character_info, unsigned int zorder)
{
	script_.show_character(character_info.first, character_info.second, std::nullopt, zorder);
}

void Game::show_character(std::string_view character_variable, unsigned int zorder)
{
	script_.show_character(character_variable, std::nullopt, std::nullopt, zorder);
}

void Game::show_character(std::pair<std::string_view, std::string_view> character_info)
{
	script_.show_character(character_info.first, character_info.second, std::nullopt, std::nullopt);
}

void Game::show_character(std::string_view character_variable)
{
	script_.show_character(character_variable, std::nullopt, std::nullopt);
}

void Game::hide_character(std::string_view character_variable)
{
	script_.hide_character(character_variable);
}

void Game::show_dialogue(std::string_view character_variable, std::string_view dialogue)
{
	script_.insert_dialogue(character_variable, dialogue);
}

void Game::show_dialogue(std::string_view dialogue)
{
	script_.insert_dialogue(constants::default_narrator_name_, dialogue);
}

void Game::show_background(std::string_view background_path, std::tuple<std::string_view, int, int, int> transition_info)
{
	script_.insert_background(background_path, utils::string_to_lower(std::get<0>(transition_info)), std::get<1>(transition_info), std::get<2>(transition_info), std::get<3>(transition_info));
}

void Game::show_background(std::string_view background_path, std::string_view transition_name)
{
	script_.insert_background(background_path, utils::string_to_lower(transition_name), std::nullopt, std::nullopt, std::nullopt);
}

void Game::show_background(std::string_view background_path)
{
	script_.insert_background(background_path);
}

void Game::show_background(Color color, std::tuple<std::string_view, int, int, int> transition_info)
{
	script_.insert_background(color, utils::string_to_lower(std::get<0>(transition_info)), std::get<1>(transition_info), std::get<2>(transition_info), std::get<3>(transition_info));
}

void Game::show_background(Color color, std::string_view transition_name)
{
	script_.insert_background(color, utils::string_to_lower(transition_name));
}

void Game::show_background(Color color)
{
	script_.insert_background(color);
}

void Game::hide_background()
{
	script_.insert_background(Color::from_rgba8(0, 0, 0));
}

void Game::play_sound(std::string_view sound_path, int channel, int fadein_length, int fadeout_length, float volume_multiplier, bool loop)
{
	script_.play_sound(sound_path, fadein_length, fadeout_length, volume_multiplier, channel, loop);
}

void Game::stop_sound(int channel, int fadeout_length)
{
	script_.stop_sound(fadeout_length, channel);
}

void Game::play_music(std::string_view music_path, int fadein_length, int fadeout_length, float volume_multiplier, bool loop)
{
	script_.play_music(music_path, fadein_length, fadeout_length, volume_multiplier, loop);
}

void Game::stop_music(int fadeout_length)
{
	script_.stop_music(fadeout_length);
}

void Game::show_choice_menu(std::string_view character_variable, std::string_view dialogue, std::initializer_list<std::pair<std::string, Script::ChoiceMenuNextDialogue>> texts)
{
	script_.insert_choice_menu(character_variable, dialogue, texts);
}

void Game::show_choice_menu(std::string_view dialogue, std::initializer_list<std::pair<std::string, Script::ChoiceMenuNextDialogue>> texts)
{
	script_.insert_choice_menu(constants::default_narrator_name_, dialogue, texts);
}

void Game::autofocus_enable()
{
	script_.insert_autofocus(true);
}

void Game::autofocus_disable()
{
	script_.insert_autofocus(false);
}

void Game::autozorder_enable()
{
	script_.insert_autozorder(true);
}

void Game::autozorder_disable()
{
	script_.insert_autozorder(false);
}

void Game::change_textbox(std::string_view character_variable, std::string_view textbox_path)
{
	script_.insert_textbox(character_variable, textbox_path);
}

void Game::change_namebox(std::string_view character_variable, std::string_view namebox_path)
{
	script_.insert_namebox(character_variable, namebox_path);
}

void Game::change_namebox_text_color(std::string_view character_variable, Color namebox_text_color)
{
	script_.insert_namebox_text_color(character_variable, namebox_text_color);
}

void Game::move_textbox(std::string_view where)
{
	script_.move_textbox(where);
}

//si la variable n'existe pas encore (lecture sur le disque), la créer et lui assigner la valeur (écriture sur le disque)
//si la variable existe déjà (lecture sur le disque) avec possiblement une autre valeur, ne rien faire
void Game::create_persistent_variable(std::string_view persistent_variable_name, PersistentType value)
{
	std::ifstream input_file(persistent_variables_filename_);
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
	
	if(data.contains(persistent_variable_name))
	{
		return;
	}
	else
	{
		std::ofstream output_file(persistent_variables_filename_);

		//TODO : utiliser std::visit
		if(std::holds_alternative<int>(value))
		{
			data[persistent_variable_name] = std::get<int>(value);
		}
		else if(std::holds_alternative<float>(value))
		{
			data[persistent_variable_name] = std::get<float>(value);
		}
		else if(std::holds_alternative<char>(value))
		{
			data[persistent_variable_name] = std::get<char>(value);
		}
		if(std::holds_alternative<std::string>(value))
		{
			data[persistent_variable_name] = std::get<std::string>(value);
		}
		output_file << data.dump(4);
		output_file.close();

		//TODO : sauvegarder dans persistent_variables_
	}
}

void Game::edit_persistent_variable(std::string_view persistent_variable_name, PersistentType new_value)
{
	std::ifstream input_file(persistent_variables_filename_);
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

	if(data.contains(persistent_variable_name))
	{
		std::ofstream output_file(persistent_variables_filename_);

		//TODO : utiliser std::visit
		if(std::holds_alternative<int>(new_value))
		{
			data[persistent_variable_name] = std::get<int>(new_value);
		}
		else if(std::holds_alternative<float>(new_value))
		{
			data[persistent_variable_name] = std::get<float>(new_value);
		}
		else if(std::holds_alternative<char>(new_value))
		{
			data[persistent_variable_name] = std::get<char>(new_value);
		}
		if(std::holds_alternative<std::string>(new_value))
		{
			data[persistent_variable_name] = std::get<std::string>(new_value);
		}
		output_file << data.dump(4);
		output_file.close();

		//TODO : sauvegarder dans persistent_variables_

		return;
	}
	else
	{
		std::cerr << "La variable persistante demandée n'existe pas => erreur !\n";
	}
}

void Game::save_persistent_variable(std::string_view persistent_variable_name)
{
	//TODO
}