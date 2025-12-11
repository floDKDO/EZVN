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

//TODO : être cohérent entre utiliser les membres ou les paramètres du constructeur (ex : renderer ou renderer_)

int main(int argc, char* argv[])
{
	(void)argc, (void)argv;

	Game game;
	game.create_character("Sayori", "img/characters/sayori.png");
	game.create_character("Monika", "img/characters/monika.png");
	game.create_character("Yuri", "img/characters/yuri.png");
	game.create_character("Natsuki", "img/characters/natsuki.png");
	game.show_character(__LINE__, "Yuri", TransformName::t41); //TODO : texte converti en l'enum dans la méthode
	game.show_character(__LINE__, "Sayori", TransformName::f42); //TODO : texte converti en l'enum dans la méthode
	game.show_character(__LINE__, "Monika", TransformName::t43); //TODO : texte converti en l'enum dans la méthode
	game.show_character(__LINE__, "Natsuki", TransformName::t44); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue(__LINE__, "Sayori", "HEYYYYYY!");
	game.hide_character(__LINE__, "Sayori");
	game.show_background(__LINE__, "img/backgrounds/class.png");
	//game.show_character(__LINE__, "Sayori", TransformName::t42); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue(__LINE__, "Elle est chiante...");
	game.hide_background(__LINE__);
	game.show_character(__LINE__, "Sayori", TransformName::f42); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue(__LINE__, "Sayori", "How are you MC?");
	game.show_background(__LINE__, "img/backgrounds/night.png");
	//game.show_character(__LINE__, "Sayori", TransformName::f31); //TODO : texte converti en l'enum dans la méthode
	game.show_character(__LINE__, "Sayori", TransformName::f42, 10); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue(__LINE__, "Sayori", "HEYYYYYY!");
	game.show_character(__LINE__, "Sayori", TransformName::t42); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue(__LINE__, "...");
	game.show_character(__LINE__, "Sayori", TransformName::f42); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue(__LINE__, "Sayori", "How are you MC?");

	game.run();

	return 0;
}