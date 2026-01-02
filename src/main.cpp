#include "game.h"

#include <iostream>

//TODO : lambdas à la place des std::bind ??

//TODO : utiliser les bons types (ex : unsigned int, std::string_view...)

//TODO : const pour les références/pointeurs en paramètre

//TODO : si valeur de wrap_length pas connue dans le constructeur ?

//TODO : supprimer les .cpp inutiles où tout le code est dans le header à cause des templates ?

//TODO : r, g, b, a ou SDL_Color ?? x, y ou {x, h} dans un Vec2 ?? Prendre aussi en charge la notation hex #FF00FFFF ??

//TODO:  être cohérent entre utiliser les membres ou les paramètres du constructeur (ex : renderer ou renderer_)

//TODO : utilité des vector.reserve(10) ??

//TODO : vérifier que je n'ai pas oublié un "override"

//TODO : nettoyer le code => retirer les includes inutiles, "_" final après le nom des membres, retirer les membres et commentaires inutiles, convention pour les valeurs des enums etc.

//TODO : peu de lisibilité avec tous les paramètres par défaut

//TODO : problème de couleur pour la couleur du premier UI de chaque menu => pas d'outline ??
//Le problème viendrait de ces deux lignes dans UiManager : current_selected_ = navigation_list_[0]; current_selected_->state_ = State::SELECTED;

//TODO : Pour l'autofocus : 
//autofocus_coloring = True
//autofocus_filter = True
//autofocus_zoom = True
//autofocus_zoom_zorder_minimum_char_requirement = 2
//autofocus_zorder = True
//autofocus_mouth = True

//TODO : si trop de code en commun entre Sound et Music, pk pas créer une classe de base Audio ??

//TODO : crash si aucun dialogue => current_unique_id_ a la valeur initiale -1 ??


int main(int argc, char* argv[])
{
	(void)argc, (void)argv;

	//TODO : inputfield qui demande le nom du joueur au premier lancement du jeu 

	//TODO : pause et window hide

	//TODO : la textbox noire apparaît brièvement pour le premier dialogue

	Game game;

	//TODO : il faudrait qu'il y ait une erreur si : 
	//- plusieurs personnages ont la même character_variable
	//- un personnage possède une character_variable vide (= "") car il serait considéré à tort comme le narrateur

	//si un son sur le même channel est lancé alors qu'un autre son était en train de se jouer, il faut stopper ce dernier
	//deux sons sur un channel différent ne s'interfèrent pas
	//fadein = facile
	//fadeout => fadeout un précédent son uniquement s'il est sur le même channel

	//play music => Fadeout gives the fadeout time for currently playing music, in seconds, while fadein gives the time it takes to fade in the new music.
	//stop music => fadeout

	game.create_character("Sayori", "Sayo", "img/characters/sayori.png", {168, 224, 255, 255}, "img/gui/sayori_textbox.png", "img/gui/sayori_namebox.png");
	game.create_character("Monika", "???", "img/characters/monika.png", {168, 255, 169, 255}, "img/gui/monika_textbox.png", "img/gui/monika_namebox.png");
	game.create_character("Yuri", "???", "img/characters/yuri.png", {184, 168, 255, 255}, "img/gui/yuri_textbox.png");
	game.create_character("Natsuki", "???", "img/characters/natsuki.png", {255, 203, 228, 255}, "img/gui/natsuki_textbox.png", "img/gui/natsuki_namebox.png");

	/*game.show_character("Yuri", "t41", 1);
	game.show_character("Sayori", "t42", 0);
	game.show_character("Monika", "t43", 2);
	game.show_character("Natsuki", "t44", 1);*/
	game.add_new_dialogue("...");
	//game.play_sound("sounds/fc_starting.ogg", 0, 2000, 3000, 10);
	game.add_new_dialogue("...");
	game.add_new_dialogue("...");
	game.add_new_dialogue("Y a personne ??");
	//game.play_sound("sounds/new_hour.ogg", 0, 2000, 3000);
	game.add_new_dialogue("...");
	//game.play_music("sounds/2.ogg", 0, 1000, 64); //TODO : volume hardcodé
	//game.show_character("Sayori", "t11");
	//game.rename_character("Sayori", "xxxxx");
	//game.add_new_dialogue("Sayori", "HEYYYYYY!");
	//game.rename_character("Sayori", "yyyyy"); 
	game.add_new_dialogue("I say that, but joining Sayori's club might be good because given the enthusiasm she has when she talks about it, it has to be nice.");
	game.show_background("img/backgrounds/class.png");
	game.autofocus_enable();
	game.play_music("sounds/3.ogg", 2, 1000); //TODO : bizarre qu'il y ait des secondes ici alors que dans tout le reste du moteur, j'utilise des ms
	game.show_character("Yuri", "t41", 1);
	game.show_character("Sayori", "t42", 0);
	game.show_character("Monika", "t43", 2);
	game.show_character("Natsuki", "t44", 1);
	game.add_new_dialogue("Elle est chiante...");
	game.show_background(129, 0, 255, 255); 
	game.add_new_dialogue("Yuri", "How are you MC?");
	game.stop_music(2000); //TODO : attention, ici il faut mettre des ms car fade_out est une méthode statique => le * 1000 effectué par stop_music n'est pas réalisé car stop_music n'est donc pas appelé
	game.rename_character("Yuri", "Yuri beta");
	game.hide_background();
	game.add_new_dialogue("Yuri", "...");
	game.show_background("img/backgrounds/corridor.png");
	game.show_character("Sayori", "t42"); 
	game.show_character("Natsuki", 10);
	game.add_new_dialogue("Natsuki", "HEYYYYYY!");
	game.add_new_dialogue("...");
	game.add_new_dialogue("Sayori", "How are you MC?");

	game.run();

	return 0;
}