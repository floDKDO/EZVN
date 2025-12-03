#include "game.h"

#include <iostream>
#include <chrono>
#include <thread>

//TODO : lambdas à la place des std::bind ??

//TODO : possibilité de juste mettre une couleur un peu plus claire pour représenter un Ui dans l'état SELECTED

//TODO : ordre des membres dans les classes par rapport aux constructeurs

//TODO : ordre des membres dans les classes selon l'ordre voulu des appels des destructeurs

//TODO : mettre en private les méthodes qui doivent l'être

//TODO : vérifier qui j'ai bien mis le suffixe "f" pour les float

//TODO : utiliser les bons types (ex : unsigned int)

//TODO : écrire les accolades des constructeurs vides comme ça : {}

//TODO : vérifier que je n'ai pas oublier explicit pour les constructeurs à un paramètre

//TODO : const pour les méthodes

//TODO : si valeur de wrap_length pas connue dans le constructeur ?

//TODO : supprimer les .cpp inutiles où tout le code est dans le header à cause des templates ?


int main(int argc, char* argv[])
{
	(void)argc, (void)argv;
	
	Game game;

	const int FPS = 60;
	const int FRAME_TIME = int((1.0f / FPS) * 1000.0f);

	Uint64 second = 0;
	Uint64 begin_current_frame = 0;
	Uint64 end_current_frame = 0;
	unsigned int frame_count = 0;

	while(game.game_running_)
	{
		begin_current_frame = SDL_GetTicks64();
		frame_count += 1;

		game.handle_events();
		game.update();
		game.draw();

		if(SDL_GetTicks64() - second >= 1000)
		{
			std::string fps_count = ", FPS: " + std::to_string(frame_count);
			game.update_fps_count(fps_count);
			second = SDL_GetTicks64();
			frame_count = 0;
		}
		end_current_frame = SDL_GetTicks64();
		std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_TIME - (end_current_frame - begin_current_frame)));
	}
	return 0;
}