#include "game.h"
#include "ingame.h"
#include "mainmenu.h"
#include "settingsmenu.h"

#include <chrono>
#include <thread>

//TODO : garder les vectors de C-pointeurs ??

Game::Game()
	: sdl_(SDL_INIT_EVERYTHING), sdl_img_(IMG_INIT_PNG | IMG_INIT_JPG), sdl_mixer_(MIX_INIT_OGG | MIX_INIT_MP3), sdl_ttf_(),
	window_(constants::game_name_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, constants::window_width_, constants::window_height_, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI),
	renderer_(window_, -1, SDL_RENDERER_PRESENTVSYNC),
	game_controller_(),
	window_icon_(constants::window_icon_), 
	main_menu_(nullptr), settings_menu_(nullptr), load_menu_(nullptr), save_menu_(nullptr), in_game_(nullptr), game_running_(true)
{
	window_.set_icon(window_icon_);

	renderer_.set_logical_size(constants::window_width_, constants::window_height_);
	renderer_.set_draw_blend_mode(SDL_BLENDMODE_BLEND);

	in_game_ = std::make_unique<InGame>(*this, renderer_);
	main_menu_ = std::make_unique<MainMenu>(*this, "img/backgrounds/night.png", renderer_);
	settings_menu_ = std::make_unique<SettingsMenu>(*this, "img/backgrounds/night.png", renderer_);
	push_state(main_menu_.get());
}

void Game::run()
{
	const Uint64 FPS = 60;
	const Uint64 FRAME_TIME = Uint64((1.0f / FPS) * 1000.0f);

	Uint64 second = 0;
	Uint64 begin_current_frame = 0;
	Uint64 end_current_frame = 0;
	unsigned int frame_count = 0;

	while(game_running_)
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
			//SDL_Delay(delay); //TODO : utiliser SDL_Delay ??
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		}
	}
}

void Game::request_push_state(GameState* state) //pour ne pas que les fonctions de callback rendent invalide current_selected_ via un appel direct à push_state qui vide le vector navigation_list_ dans lequel current_selected_ prend sa valeur
{
	RequestedAction requested_action = {Action::push, state};
	requested_actions_.push(requested_action);
}

void Game::request_pop_state()
{
	RequestedAction requested_action = {Action::pop, nullptr};
	requested_actions_.push(requested_action);
}

void Game::quit_game()
{
	game_running_ = false;
}

void Game::handle_requests()
{
	while(!requested_actions_.empty())
	{
		RequestedAction top = requested_actions_.top();
		if(top.action == Action::push)
		{
			push_state(top.game_state);
		}
		else if(top.action == Action::pop)
		{
			pop_state();
		}
		requested_actions_.pop();
	}
}

void Game::push_state(GameState* state)
{
	states_.push(state);
}

void Game::pop_state()
{
	states_.pop();
}

GameState* Game::get_state() const
{
	return states_.top();
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
					game_running_ = false;
				}
				break;

			case SDL_KEYDOWN:
				switch(e.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						game_running_ = false;
						break;

					default:
						break;
				}
				break;

			default:
				break;
		}
		get_state()->handle_events(e);
	}
}

void Game::draw()
{
	renderer_.set_draw_color(0, 0, 0, 255); //TODO : à la place, une fonction de callback qui est appelée quand la fenêtre est redimensionnée ??
	renderer_.clear();
	get_state()->draw(renderer_);
	renderer_.present();
}

void Game::update()
{
	handle_requests();
	get_state()->update();
}

void Game::update_fps_count(const std::string_view fps) const
{
	window_.set_title(std::string(constants::game_name_) + std::string(fps));
}

void Game::create_character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, const SDL_Color namebox_font_color, const std::string_view textbox_path, const std::string_view namebox_path)
{
	dynamic_cast<InGame*>(in_game_.get())->add_character(character_variable, character_name, character_path, renderer_, namebox_font_color, textbox_path, namebox_path);
}

void Game::rename_character(const std::string_view character_variable, const std::string_view new_character_name)
{
	dynamic_cast<InGame*>(in_game_.get())->rename_character(character_variable, new_character_name);
}

void Game::show_character(const std::string_view character_variable, const std::optional<std::string> transform_name, const std::optional<int> zorder)
{
	dynamic_cast<InGame*>(in_game_.get())->show_character(character_variable, transform_name, zorder);
}

void Game::show_character(const std::string_view character_variable, const std::optional<int> zorder)
{
	dynamic_cast<InGame*>(in_game_.get())->show_character(character_variable, zorder);
}

void Game::hide_character(const std::string_view character_variable)
{
	dynamic_cast<InGame*>(in_game_.get())->hide_character(character_variable);
}

void Game::add_new_dialogue(const std::string_view character_variable, const std::string_view dialogue)
{
	dynamic_cast<InGame*>(in_game_.get())->insert_dialogue(character_variable, dialogue);
}

void Game::add_new_dialogue(const std::string_view dialogue)
{
	dynamic_cast<InGame*>(in_game_.get())->insert_dialogue("", dialogue); //TODO : mettre nullptr car j'utiliserai Character* à la place d'une std::string_view ???
}

void Game::show_background(const std::string_view background_path)
{
	dynamic_cast<InGame*>(in_game_.get())->insert_background(background_path);
}

void Game::show_background(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a)
{
	dynamic_cast<InGame*>(in_game_.get())->insert_background(r, g, b, a);
}

void Game::hide_background()
{
	dynamic_cast<InGame*>(in_game_.get())->insert_background(0, 0, 0, 255);
}

void Game::play_sound(const std::string_view sound_path, int channel, int fadein_length, int fadeout_length, int volume, bool loop)
{
	dynamic_cast<InGame*>(in_game_.get())->insert_sound(sound_path, fadein_length, fadeout_length, volume, channel, loop);
}

void Game::stop_sound(int channel, int fadeout_length)
{
	dynamic_cast<InGame*>(in_game_.get())->insert_sound("", 0, fadeout_length, 128, channel, false); //TODO : volume, channel et loop hardcodés
}

void Game::play_music(const std::string_view music_path, int fadein_length, int fadeout_length, int volume, bool loop)
{
	dynamic_cast<InGame*>(in_game_.get())->insert_music(music_path, fadein_length, fadeout_length, volume, loop);
}

void Game::stop_music(int fadeout_length)
{
	dynamic_cast<InGame*>(in_game_.get())->insert_music("", 0, fadeout_length, 128, true); //TODO : volume et loop hardcodés
}

void Game::autofocus_enable()
{
	dynamic_cast<InGame*>(in_game_.get())->insert_autofocus(true);
}

void Game::autofocus_disable()
{
	dynamic_cast<InGame*>(in_game_.get())->insert_autofocus(false);
}