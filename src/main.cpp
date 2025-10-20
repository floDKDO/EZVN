#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "image.h"
#include "sound.h"
#include "music.h"
#include "button.h"
#include "textbutton.h"
#include "toggle.h"
#include "inputfield.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

//TODO : la classe Image est générale pour toutes les images, et créer une classe Sprite/Character/... pour les personnages
//TODO : corriger les warnings
//TODO : =delete
//TODO : ordre des attributs dans les classes
//TODO : retirer les include inutiles
//TODO : convention dans l'ordre des include
//TODO : init des arg des constructeurs sur plusieurs lignes si nécessaire
//TODO : ajouter un compteur de FPS (et limiter les FPS / caper les mouvements selon les FPS)


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

bool cmp(const Image& a, const Image& b)
{
	return a.zorder < b.zorder;
}


void button_function(Ui* ui)
{
	std::cout << "Clicked !" << std::endl;
}

void inputfield_function(Ui* ui)
{
	std::cout << dynamic_cast<Inputfield*>(ui)->text.text << std::endl;
}


int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG); //TODO : quels flags ?
	Mix_Init(MIX_INIT_OGG); //TODO : quels flags ?
	chk_SDL(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_CHANNELS, 1024), Mix_GetError());
	TTF_Init();

	SDL_StartTextInput();

	SDL_Window* window;
	nchk_SDL(window = SDL_CreateWindow("EZVN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE), SDL_GetError()); //TODO : constantes

	//TODO : l'utiliser en extern dans les autres classes ?
	SDL_Renderer* renderer;
	nchk_SDL(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_TARGETTEXTURE), SDL_GetError()); //TODO : quels flags ? (SDL_RENDERER_TARGETTEXTURE utile ?)
	chk_SDL(SDL_RenderSetLogicalSize(renderer, 1280, 720), SDL_GetError()); //TODO : constantes
	chk_SDL(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND), SDL_GetError()); //TODO : utile ??

	//std::vector<Image> images = {Image("img/ai-blob-walk.gif", renderer, 1), Image("img/yuri_tea.png", renderer, 2)};
	//std::vector<Image> images = {Image("img/yuri_tea.png", renderer, 1)}; //=> problème : le destructeur est appelé immédiatement

	Image yuri("img/yuri_tea.png", 100, 100, renderer, 1);
	Image blob("img/ai-blob-walk.gif", 500, 200, renderer, 1);
	std::vector<Image> images;
	images.reserve(10);
	images.push_back(yuri);
	images.push_back(blob);

	//TODO : utiliser "const référence" pour des gros objets "If you don’t want to change the object passed and it is big, call by const reference; e.g., void f(const X&);."

	std::stable_sort(images.begin(), images.end(), &cmp);

	Ui* button = new Button("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", 200, 200, renderer, &button_function);
	Ui* textbutton = new TextButton("My text", {255, 255, 255, 255}, {255, 0, 0, 255}, {0, 0, 255, 255}, 500, 500, renderer, &button_function);
	Ui* toggle = new Toggle("img/button_normal.png", "img/button_selected.png", "img/button_clicked.png", "img/checked.png", 100, 600, true, renderer, &button_function);
	Ui* inputfield = new Inputfield("img/inputfield.png", {0, 0, 0, 255}, 100, 0, renderer, &inputfield_function);
	//TODO : ne pas oublier les delete

	Uint64 time_step = SDL_GetTicks64(); 

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
			textbutton->handle_events(e);
			toggle->handle_events(e);
			inputfield->handle_events(e);
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
		inputfield->update(time_step);

		SDL_RenderPresent(renderer);
	}

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