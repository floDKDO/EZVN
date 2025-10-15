#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "image.h"

#include <iostream>
#include <chrono>
#include <thread>


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

	Image yuri("img/yuri_tea.png", renderer);

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

						case SDLK_a:
							yuri.flip_horizontally();
							break;

						case SDLK_z:
							yuri.flip_vertically();
							break;

						case SDLK_e:
							yuri.flip_normal();
							break;

						case SDLK_h:
							yuri.hide(renderer);
							break;

						case SDLK_s:
							yuri.show(renderer);
							break;

						case SDLK_UP:
							yuri.angle += 1;
							break;

						case SDLK_DOWN:
							yuri.angle -= 1;
							break;

						case SDLK_LEFT:
							yuri.position(yuri.rect.x - 10, yuri.rect.y - 10);
							break;

						case SDLK_RIGHT:
							yuri.position(yuri.rect.x + 10, yuri.rect.y + 10);
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
		}
		
		SDL_RenderClear(renderer);
		SDL_RenderCopyEx(renderer, yuri.texture, nullptr, &(yuri.rect), yuri.angle, nullptr, yuri.flip);
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