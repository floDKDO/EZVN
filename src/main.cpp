#include "game.h"
#include "character.h"
#include "transform.h"

#include <iostream>

//TODO : lambdas à la place des std::bind ??

//TODO : utiliser les bons types (ex : unsigned int)

//TODO : const pour les références/pointeurs en paramètre

//TODO : si valeur de wrap_length pas connue dans le constructeur ?

//TODO : supprimer les .cpp inutiles où tout le code est dans le header à cause des templates ?

//TODO : r, g, b, a ou SDL_Color ?? x, y ou {x, h} dans un Vec2 ??

//TODO:  être cohérent entre utiliser les membres ou les paramètres du constructeur (ex : renderer ou renderer_)

//TODO : utilité des vector.reserve(10) ??

//TODO : le full screen doit également maximiser la fenêtre dans le cas où celle-ci avait été redimensionnée ??

//TODO : vérifier que je n'ai pas oublié un "override"


int main(int argc, char* argv[])
{
	(void)argc, (void)argv;

	//TODO : pause et window hide

	Game game;
	game.create_character("Sayori", "img/characters/sayori.png");
	game.create_character("Monika", "img/characters/monika.png");
	game.create_character("Yuri", "img/characters/yuri.png");
	game.create_character("Natsuki", "img/characters/natsuki.png");
	game.show_character("Yuri", TransformName::t41, 1); //TODO : texte converti en l'enum dans la méthode
	game.show_character("Sayori", TransformName::f42, 0); //TODO : texte converti en l'enum dans la méthode
	game.show_character("Monika", TransformName::t43, 1); //TODO : texte converti en l'enum dans la méthode
	game.show_character("Natsuki", TransformName::t44, 1); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue("Sayori", "HEYYYYYY!");
	game.show_character("Sayori", TransformName::t42); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue("I say that, but joining Sayori's club might be good because given the enthusiasm she has when she talks about it, it has to be nice.");
	game.show_background("img/backgrounds/class.png");
	game.hide_character("Sayori"); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue("Elle est chiante...");
	game.hide_background();
	game.show_background(129, 0, 255, 255);
	game.show_character("Yuri", TransformName::f41); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue("Yuri", "How are you MC?");
	game.show_character("Yuri", TransformName::t41); //TODO : texte converti en l'enum dans la méthode
	game.show_background("img/backgrounds/night.png");
	//game.show_character("Sayori", TransformName::f31); //TODO : texte converti en l'enum dans la méthode
	game.show_character("Monika", TransformName::f43, 10); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue("Monika", "HEYYYYYY!");
	game.show_character("Monika", TransformName::t43); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue("...");
	game.show_character("Sayori", TransformName::f42); //TODO : texte converti en l'enum dans la méthode
	game.add_new_dialogue("Sayori", "How are you MC?");

	game.run();

	return 0;
}