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
	game.create_character("Sayori", "img/characters/sayori.png");
	//game.create_character("Monika", "img/characters/monika.png");
	//game.create_character("Yuri", "img/characters/yuri.png");
	//game.create_character("Natsuki", "img/characters/natsuki.png");
	game.show_character(__LINE__, "Sayori", TransformName::f11); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue(__LINE__, "Sayori", "HEYYYYYY!");
	game.show_character(__LINE__, "Sayori", TransformName::f44); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue(__LINE__, "Elle est chiante...");
	game.add_new_dialogue(__LINE__, "Sayori", "How are you MC?");
	game.show_character(__LINE__, "Sayori", TransformName::f31); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue(__LINE__, "Sayori", "HEYYYYYY!");
	game.show_character(__LINE__, "Sayori", TransformName::f22); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue(__LINE__, "Sayori", "How are you MC?");

	game.run();

	return 0;
}