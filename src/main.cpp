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
// Exceptions pour les erreurs "graves" => SDL_CreateWindow(), SDL_CreateRenderer() ??

//TODO : corriger le problème du time_step et modifier la valeur de last_time dans le update() de toutes les classes

//TODO : time_step <=> begin_current_frame ou end_current_frame ??

//TODO : lambdas à la place des std::bind ??

//TODO : possibilité de juste mettre une couleur un peu plus claire pour représenter un Ui dans l'état SELECTED

//TODO : ordre des membres dans les classes par rapport aux constructeurs

//TODO : ordre des membres dans les classes selon l'ordre voulu des appels des destructeurs

//TODO : ne créer qu'une seule font ??

//TODO : mettre en private les méthodes qui doivent l'être

//TODO : voir pour le bug où deux Ui peuvent être sélectionnés en même temps => se déclenche en cas de défilements rapides avec la souris sur les Checkable d'un CheckableGroup, ne se déclenche pas sur les autres Ui
//Raison : d'baord exécution update (boucle en entier) du CheckableGroup avant que le handle_events puisse appliquer la protection

//TODO : bu si sélection d'un Checkable d'un CheckableGroup avec le clavier puis qu'on en sélectionne un autre avec la souris => les deux seront SELECTED


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

	while(game.game_running_)
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
			game.window_.set_title(window_title);
			second = SDL_GetTicks64();
			frame_count = 0;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_TIME - (end_current_frame - begin_current_frame)));
		 
		//std::cout << "(MAIN)Valeur de time_step : " << time_step << std::endl;
		time_step = SDL_GetTicks64();
	}
	return 0;
}