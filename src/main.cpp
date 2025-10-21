#include "image.h"
#include "sound.h"
#include "music.h"
#include "button.h"
#include "textbutton.h"
#include "toggle.h"
#include "inputfield.h"
#include "texttoggle.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>


//TODO : =delete, explicit, final


void chk_SDL(const int return_value, const char* error_string)
{
	if(return_value < 0)
	{
		std::cerr << error_string << std::endl;
		exit(EXIT_FAILURE);
	}
}

void nchk_SDL(const void* return_value, const char* error_string)
{
	if(return_value == nullptr)
	{
		std::cerr << error_string << std::endl;
		exit(EXIT_FAILURE);
	}
}

void button_function(Ui* ui)
{
	(void)ui;
	std::cout << "Clicked !" << std::endl;
}

void inputfield_function(Ui* ui)
{
	std::cout << dynamic_cast<Inputfield*>(ui)->text.text << std::endl;
}


int main(int argc, char* argv[])
{
	(void)argc, (void)argv;
	SDL_Init(SDL_INIT_EVERYTHING); 
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
	chk_SDL(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_CHANNELS, 2048), Mix_GetError()); 
	TTF_Init(); 

	SDL_StartTextInput();

	SDL_GameController* controller = SDL_GameControllerOpen(0);
	SDL_GameControllerEventState(SDL_ENABLE);

	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;
	static const int FPS = 60;
	static const int FRAME_TIME = int((1.0f / FPS) * 1000.0f);

	SDL_Window* window;
	nchk_SDL(window = SDL_CreateWindow("EZVN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE), SDL_GetError()); 

	//TODO : l'utiliser en extern dans les autres classes ?
	SDL_Renderer* renderer;
	nchk_SDL(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC), SDL_GetError()); 
	chk_SDL(SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT), SDL_GetError()); 
	chk_SDL(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND), SDL_GetError()); 

	//std::vector<Image> images = {Image("img/ai-blob-walk.gif", renderer, 1), Image("img/yuri_tea.png", renderer, 2)};
	//std::vector<Image> images = {Image("img/yuri_tea.png", renderer, 1)}; //=> problème : le destructeur est appelé immédiatement

	Image yuri("img/yuri_tea.png", 100, 100, renderer, 1);
	Image blob("img/ai-blob-walk.gif", 500, 200, renderer, 1);
	std::vector<Image> images;
	images.reserve(10);
	images.push_back(yuri);
	images.push_back(blob);

	//TODO : utiliser "const référence" pour des gros objets "If you don’t want to change the object passed and it is big, call by const reference; e.g., void f(const X&);."

	std::stable_sort(images.begin(), images.end(), 
		[](const Image& a, const Image& b)
		{
			return a.zorder < b.zorder;
		}); 

	Ui* button = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 200, 200, renderer, &button_function);
	Ui* textbutton = new TextButton("My text", {255, 255, 255, 255}, {255, 0, 0, 255}, {0, 0, 255, 255}, 500, 500, renderer, &button_function);
	Ui* toggle = new Toggle("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", "img/checked.png", 100, 600, true, renderer, &button_function);
	Ui* inputfield = new Inputfield("img/inputfield.png", {0, 0, 0, 255}, 10, 100, 0, renderer, &inputfield_function);
	Ui* texttoggle = new TextToggle("Speed mode", {127, 127, 127, 255}, {180, 180, 180, 255}, {255, 0, 0, 255}, 700, 150, false, renderer, &button_function);

	Uint64 time_step = SDL_GetTicks64(); 
	Uint64 second = 0;
	Uint64 begin_current_frame = 0;
	Uint64 end_current_frame = 0;

	unsigned int frame_count = 0;

	bool game_running = true;
	while(game_running)
	{
		begin_current_frame = SDL_GetTicks64();
		frame_count += 1;

		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_WINDOWEVENT:
					if(e.window.event == SDL_WINDOWEVENT_CLOSE)
					{
						game_running = false;
					}
					break;

				case SDL_KEYDOWN:
					switch(e.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							game_running = false;
							break;

						default:
							break;
					}
					break;

				case SDL_KEYUP:
					break;
					
				default:
					break;
			}

			button->handle_events(e);
			textbutton->handle_events(e);
			toggle->handle_events(e);
			inputfield->handle_events(e);
			texttoggle->handle_events(e);
		}
		
		SDL_RenderClear(renderer);

		for(Image& i : images) //TODO : sans référence sur i, le destructeur de chaque élément de images est appelé à chaque tour de boucle
		{
			i.draw(renderer);
		}

		button->draw(renderer);
		textbutton->draw(renderer);
		toggle->draw(renderer);
		inputfield->draw(renderer);
		texttoggle->draw(renderer);

		button->update(time_step);
		textbutton->update(time_step);
		toggle->update(time_step);
		inputfield->update(time_step);
		texttoggle->update(time_step);

		SDL_RenderPresent(renderer);

		end_current_frame = SDL_GetTicks64();
		if(SDL_GetTicks64() - second >= 1000)
		{
			std::string window_title = "EZVN, FPS: " + std::to_string(frame_count);
			SDL_SetWindowTitle(window, window_title.c_str());
			second = SDL_GetTicks64();
			frame_count = 0;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_TIME - (end_current_frame - begin_current_frame)));
	}

	delete button;
	delete textbutton;
	delete toggle;
	delete inputfield;

	if(controller != nullptr)
		SDL_GameControllerClose(controller);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_StopTextInput();

	TTF_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}