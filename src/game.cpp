#include "game.h"
#include "constants.h"
#include "ingame.h"
#include "mainmenu.h"
#include "settingsmenu.h"

#include <chrono>
#include <thread>

//TODO : garder les vectors de C-pointeurs ??

Game::Game()
	: sdl_(SDL_INIT_EVERYTHING), sdl_img_(IMG_INIT_PNG | IMG_INIT_JPG), sdl_mixer_(MIX_INIT_OGG | MIX_INIT_MP3), sdl_ttf_(),
	window_("EZVN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, constants::window_width_, constants::window_height_, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI),
	renderer_(window_, -1, SDL_RENDERER_PRESENTVSYNC),
	game_controller_(),
	window_icon_(constants::window_icon_), ui_manager_(),
	main_menu_(nullptr), settings_menu_(nullptr), load_menu_(nullptr), save_menu_(nullptr), in_game_(nullptr), game_running_(true)
{
	window_.set_icon(window_icon_);

	renderer_.set_logical_size(constants::window_width_, constants::window_height_);
	renderer_.set_draw_blend_mode(SDL_BLENDMODE_BLEND);

	in_game_ = std::make_unique<InGame>(*this, renderer_);
	main_menu_ = std::make_unique<MainMenu>(*this, "img/backgrounds/night.png", renderer_);
	settings_menu_ = std::make_unique<SettingsMenu>(*this, "img/backgrounds/night.png", renderer_);
	//create_main_menu();
	//create_settings_menu();
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

	//mettre le premier dialogue dans la Textbox => plus petite clef
	InGame* in_game_ptr = dynamic_cast<InGame*>(in_game_.get());
	in_game_ptr->textbox_.set_initial_dialogue(in_game_ptr->dialogues_.begin()->second.first, in_game_ptr->dialogues_.begin()->second.second);

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
		//Uint64 delay = FRAME_TIME - elapsed;

		if(FRAME_TIME > elapsed)
		{
			//SDL_Delay(delay); //TODO : utiliser SDL_Delay ??
			std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_TIME - (end_current_frame - begin_current_frame)));
		}
	}
}

/*void Game::create_main_menu()
{
	std::vector<std::unique_ptr<Ui>> ui_elements;
	ui_elements.reserve(10);

	//ici, std::placeholders::_1 est nécessaire car l'appel à la fonction de callback (ce qui est retourné par std::bind) est de la forme : f(this) => on spécifie l'argument lors de l'appel au callable et pas directement sa valeur dans std::bind
	ui_elements.push_back(std::make_unique<TextButton>("Play", 600, 200, renderer_, std::bind(&Game::play_function, this, std::placeholders::_1)));
	ui_elements.push_back(std::make_unique<TextButton>("Settings", 600, 350, renderer_, std::bind(&Game::settings_function, this, std::placeholders::_1)));
	ui_elements.push_back(std::make_unique<TextButton>("Quit", 600, 500, renderer_, "Are you sure you would like to close the game?", std::bind(&Game::confirmationpopup_quit_function, this, std::placeholders::_1)));

	main_menu_ = std::make_unique<MenuState>("img/backgrounds/night.png", std::move(ui_elements), renderer_);
}*/

/*void Game::create_settings_menu()
{
	//std::unique_ptr<Ui> togglegroup = std::make_unique<TextToggleGroup>(2, "Display", std::vector<std::string>{"Windowed", "Fullscreen"}, 50, 100, true, renderer.fetch(), std::vector<std::function<void(Ui* ui)>>{std::bind(&Game::texttoggle_windowed_function, this, std::placeholders::_1), std::bind(&Game::texttoggle_full_screen_function, this, std::placeholders::_1)});

	std::vector<std::unique_ptr<Ui>> ui_elements;
	ui_elements.reserve(10);
	 
	ui_elements.push_back(std::make_unique<TextButton>("Return", 200, 500, renderer_, std::bind(&Game::previous_menu_function, this, std::placeholders::_1)));
	ui_elements.push_back(std::make_unique<Slider>(0, 100, 800, 200, "Sound effect", renderer_, std::bind(&Game::slider_sound_function, this, std::placeholders::_1)));
	ui_elements.push_back(std::make_unique<Slider>(0, 100, 450, 200, "Music effect", renderer_, std::bind(&Game::slider_music_function, this, std::placeholders::_1)));
	ui_elements.push_back(std::make_unique<Slider>(30, 60, 625, 350, "Text speed", renderer_, std::bind(&Game::slider_text_function, this, std::placeholders::_1)));
	ui_elements.push_back(std::make_unique<TextToggleGroup<2>>("Display", std::vector<std::string>{"Windowed", "Fullscreen"}, 50, 100, true, renderer_, std::vector<std::function<void(Ui* ui)>>{std::bind(&Game::texttoggle_windowed_function, this, std::placeholders::_1), std::bind(&Game::texttoggle_full_screen_function, this, std::placeholders::_1)}));
	//ui_elements.push_back(std::make_unique<CheckboxGroup<2>>("Display", std::vector<std::string>{"Windowed", "Fullscreen"}, 50, 100, true, renderer_, std::vector<std::function<void(Ui* ui)>>{std::bind(&Game::texttoggle_windowed_function, this, std::placeholders::_1), std::bind(&Game::texttoggle_full_screen_function, this, std::placeholders::_1)}));
	settings_menu_ = std::make_unique<MenuState>("img/backgrounds/night.png", std::move(ui_elements), renderer_);
}*/

void Game::push_state(GameState* state)
{
	states_.push(state);
	ui_manager_.set_elements(states_.top()->ui_elements_);
}

void Game::pop_state()
{
	states_.pop();
	ui_manager_.set_elements(states_.top()->ui_elements_);
}

GameState* Game::get_state() const
{
	if(states_.empty())
		std::cerr << "VIDE!\n";
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
		ui_manager_.handle_events(e);
	}
}

void Game::draw()
{
	renderer_.clear();
	get_state()->draw(renderer_);
	renderer_.present();
}

void Game::update()
{
	get_state()->update();
	ui_manager_.update();
}

void Game::update_fps_count(const std::string fps) const
{
	window_.set_title(constants::game_name_ + fps);
}

void Game::create_character(const std::string character_name, const std::string character_path)
{
	dynamic_cast<InGame*>(in_game_.get())->add_character(character_name, character_path, renderer_);
}

void Game::show_character(unsigned int line_number, const std::string character_name, const TransformName transform_name)
{
	InGame* in_game_ptr = dynamic_cast<InGame*>(in_game_.get());
	Character* character = in_game_ptr->get_character(character_name);
	//character->is_visible_ = true;
	//character->set_transform(transform_name);

	in_game_ptr->characters_transforms_.insert({line_number, {character, transform_name}}); //TODO : make_pair ou accolades ??
}

void Game::hide_character(unsigned int line_number, const std::string character_name)
{
	//TODO : hide_character
}

void Game::add_new_dialogue(unsigned int line_number, const std::string character_name, const std::string dialogue)
{
	InGame* in_game_ptr = dynamic_cast<InGame*>(in_game_.get());

	Character* character = in_game_ptr->get_character(character_name);
	//TODO : il y a une copie de l'unique_ptr ??
	if(character != nullptr)
	{
		in_game_ptr->dialogues_.insert({line_number, {dialogue, character}});
	}
}

void Game::add_new_dialogue(unsigned int line_number, const std::string dialogue)
{
	InGame* in_game_ptr = dynamic_cast<InGame*>(in_game_.get());
	in_game_ptr->dialogues_.insert({line_number, {dialogue, nullptr}});
}