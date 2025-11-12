#include "image.h"
#include "sound.h"
#include "music.h"
#include "button.h"
#include "textbutton.h"
#include "toggle.h"
#include "inputfield.h"
#include "texttoggle.h"
#include "slider.h"
#include "menu.h"
#include "game.h"

//TODO : pas besoin de tous ces includes

#include <iostream>
#include <chrono>
#include <thread>


//TODO : =delete, explicit, final

//TODO : utiliser les types SDL2 encapsulés
//TODO : est-ce que je dois utiliser les types SDL2 encapsulés dans les classes des types encapsulés ? (ex : SDL_Renderer qui a besoin d'une SDL_Window)
//TODO : ne pas mettre de unique_ptr pour SDL_Renderer* des classes autres que Game


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

int main(int argc, char* argv[])
{
	(void)argc, (void)argv;
	
	Game game;

	const int FPS = 60;
	const int FRAME_TIME = int((1.0f / FPS) * 1000.0f);

	Uint64 time_step = SDL_GetTicks64(); 
	Uint64 second = 0;
	Uint64 begin_current_frame = 0;
	Uint64 end_current_frame = 0;
	unsigned int frame_count = 0;

	while(game.game_running)
	{
		begin_current_frame = SDL_GetTicks64();
		frame_count += 1;

		game.handle_events();
		game.update(time_step);
		game.draw();

		end_current_frame = SDL_GetTicks64();
		if(SDL_GetTicks64() - second >= 1000)
		{
			std::string window_title = "EZVN, FPS: " + std::to_string(frame_count);
			game.window.set_title(window_title);
			second = SDL_GetTicks64();
			frame_count = 0;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_TIME - (end_current_frame - begin_current_frame)));
	}
	return 0;
}