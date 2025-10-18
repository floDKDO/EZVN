#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "image.h"
#include "sound.h"
#include "music.h"
#include "button.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>


void chk_SDL(int return_value, const char* error_string)
{
	if(return_value < 0)
	{
		std::cerr << error_string << std::endl;
		exit(EXIT_FAILURE);
	}
}

void nchk_SDL(void* return_value, const char* error_string)
{
	if(return_value == nullptr)
	{
		std::cerr << error_string << std::endl;
		exit(EXIT_FAILURE);
	}
}

bool cmp(const Image& a, const Image& b)
{
	return a.zorder < b.zorder;
}


void button_function(void)
{
	std::cout << "Clicked !" << std::endl;
}


int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	Mix_Init(MIX_INIT_OGG);
	chk_SDL(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_CHANNELS, 1024), Mix_GetError());
	TTF_Init();

	SDL_Window* window;
	nchk_SDL(window = SDL_CreateWindow("EZVN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE), SDL_GetError());

	SDL_Renderer* renderer;
	nchk_SDL(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_TARGETTEXTURE), SDL_GetError());
	chk_SDL(SDL_RenderSetLogicalSize(renderer, 1280, 720), SDL_GetError());
	chk_SDL(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND), SDL_GetError());

	//std::vector<Image> images = {Image("img/ai-blob-walk.gif", renderer, 1), Image("img/yuri_tea.png", renderer, 2)};
	//std::vector<Image> images = {Image("img/yuri_tea.png", renderer, 1)}; //=> problème : le destructeur est appelé immédiatement

	Image yuri("img/yuri_tea.png", renderer, 1);
	Image blob("img/ai-blob-walk.gif", renderer, 1);
	std::vector<Image> images;
	images.reserve(10);
	images.push_back(yuri);
	images.push_back(blob);

	std::stable_sort(images.begin(), images.end(), &cmp);

	Ui* button = new Button("img/button.png", 200, 200, renderer, &button_function);

	bool game_running = true;
	while(game_running)
	{
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
		}
		
		SDL_RenderClear(renderer);

		for(Image& i : images) //TODO : sans référence sur i, le destructeur de chaque élément de images est appelé à chaque tour de boucle
		{
			i.draw(renderer);
		}

		button->draw(renderer);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}