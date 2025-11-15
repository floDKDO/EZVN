#include "game.h"

#include <iostream>
#include <chrono>
#include <thread>

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

// TODO : gérer les erreurs des fonctions SDL. 
// Idée : ne pas crash systématiquement en cas d'erreur (ex avec Ren'Py qui affiche une image noire si l'image qu'on veut afficher n'est pas trouvée)
// => avoir une image par défaut pour chaque classe d'images (Button, Inputfield, Character...) => détectable par le dossier qui contient l'image (gui pour Button etc., characters pour Character...)
// De plus, toutes les erreurs n'ont pas le même impact (ex : une erreur pour SDL_RenderClear() n'est pas grave)
// Il faudrait utiliser SDL_Log à la place de fprintf(stderr, ...), et éventuellement SDL_ShowSimpleMessageBox
// Préalable : ne pas afficher toutes les erreurs pour le mode Release

//TODO : corriger le problème du time_step et modifier la valeur de last_time dans le update() de toutes les classes

//TODO : time_step <=> end_current_frame ??
 
//TODO : mettre le SDL_Renderer* en attribut private dans les classes 


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
		 
		//std::cout << "(MAIN)Valeur de time_step : " << time_step << std::endl;
		time_step = SDL_GetTicks64();
	}
	return 0;
}