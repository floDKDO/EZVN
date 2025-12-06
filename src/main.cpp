#include "game.h"
#include "character.h"
#include "transform.h"

#include <iostream>

//TODO : lambdas à la place des std::bind ??

//TODO : possibilité de juste mettre une couleur un peu plus claire pour représenter un Ui dans l'état SELECTED

//TODO : utiliser les bons types (ex : unsigned int)

//TODO : const pour les références/pointeurs en paramètre

//TODO : si valeur de wrap_length pas connue dans le constructeur ?

//TODO : supprimer les .cpp inutiles où tout le code est dans le header à cause des templates ?

//TODO : r, g, b, a ou SDL_Color ?? x, y ou {x, h} dans un Vec2 ??

int main(int argc, char* argv[])
{
	(void)argc, (void)argv;
	
	Game game;
	game.add_character("Sayori", "img/characters/sayori.png");
	game.add_new_dialogue("HEYYYYYY!", "Sayori", TransformName::t11); //TODO : texte convertit en l'enum dans la méthode
	game.add_new_dialogue("How are you MC?", "Sayori", TransformName::t33);
	game.add_new_dialogue("HEYYYYYY!", "Sayori", TransformName::f41); //TODO : texte convertit en l'enum dans la méthode
	game.add_new_dialogue("How are you MC?", "Sayori", TransformName::t21);
	game.run();

	return 0;
}