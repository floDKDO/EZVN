#include "game.h"

#include <iostream>

//TODO : lambdas à la place des std::bind ??

//TODO : utiliser les bons types (ex : unsigned int, std::string_view...)

//TODO : const pour les références/pointeurs en paramètre

//TODO : si valeur de wrap_length pas connue dans le constructeur ?

//TODO : supprimer les .cpp inutiles où tout le code est dans le header à cause des templates ?

//TODO:  être cohérent entre utiliser les membres ou les paramètres du constructeur (ex : renderer ou renderer_)

//TODO : utilité des vector.reserve(10) ??

//TODO : vérifier que je n'ai pas oublié un "override"

//TODO : placer les définitions de types, les structs et enums au début des classes et les placer dans la bonne visibilité (ex : public)

//TODO : membre static => static inline ou static constexpr pour attribuer une valeur directement dans le .h

//TODO : utiliser des free functions si possible

//TODO : anonymous namespace au lieu de static pour des fonctions static et des variables globales static propres à une translation unit

//TODO : nettoyer le code => retirer les includes inutiles (attention que je n'inclus pas le même header dans un .h et son .cpp associé + utiliser des forward declarations quand c'est possible), 
//                        => "_" final après le nom des membres, retirer les membres et commentaires inutiles, convention pour les valeurs des enums etc.

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

//TODO : crash si aucun dialogue => current_script_index_ a la valeur initiale -1 ??

//TODO : dans la member initialization list, ne pas écrire : transform_(Transform(transform_name)) mais transform_(transform_name)

//TODO : il ne faudrait pas utiliser std::string_view en tant que clef dans une std::map ou std::unordered_map

//TODO : mettre des constructeurs par défaut dans les structs pour simplifier l'initialisation de leurs membres 

//TODO : créer des variables pour les éléments de tableau utilisés à plusieurs endroits dans une même méthode

//TODO : convention de nommage pour les underscores dans les noms de fichier

//TODO : bloquer le changement de dialogues (auto/skip) quand on appuie sur Settings

//TODO : convention pour les strings vides ("" ou std::string() ??)


#include "Color/str.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) //mandatory to use argc and argv for SDL2
{
	//TODO : inputfield qui demande le nom du joueur au premier lancement du jeu => nécessite les variables persistantes

	//TODO : pause et window hide

	//TODO : la textbox noire apparaît brièvement pour le premier dialogue, de même que des anciens noms dans la namebox

	/*Color c1(Color::from_rgba8(120, 200, 140, 100));
	std::cout << "c1: " << int(c1.r_) << ", " << int(c1.g_) << ", " << int(c1.b_) << ", " << int(c1.a_) << std::endl;
	Color c2(Color::from_rgba(0.2, 0.5, 0.3, 0.75));
	std::cout << "c2: " << int(c2.r_) << ", " << int(c2.g_) << ", " << int(c2.b_) << ", " << int(c2.a_) << std::endl;
	Color c3(Color::from_hsva8(342, 80, 100, 220));
	std::cout << "c3: " << int(c3.r_) << ", " << int(c3.g_) << ", " << int(c3.b_) << ", " << int(c3.a_) << std::endl;
	Color c4(Color::from_hsva(342, 0.2, 0.5, 0.8));
	std::cout << "c4: " << int(c4.r_) << ", " << int(c4.g_) << ", " << int(c4.b_) << ", " << int(c4.a_) << std::endl;
	Color c5(Color::from_hex("#ff00ff"));
	std::cout << "c5: " << int(c5.r_) << ", " << int(c5.g_) << ", " << int(c5.b_) << ", " << int(c5.a_) << std::endl;
	Color c6(Color::from_string("DARK Red"));
	std::cout << "c6: " << int(c6.r_) << ", " << int(c6.g_) << ", " << int(c6.b_) << ", " << int(c6.a_) << std::endl;*/


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

	game.create_character("Sayori", "Sayo", "img/characters/sayori.png", Color::from_rgba8(168, 224, 255), "img/gui/sayori_textbox.png", "img/gui/sayori_namebox.png");
	game.create_character("Monika", "???", "img/characters/monika.png", Color::from_rgba8(168, 255, 169), "img/gui/monika_textbox.png", "img/gui/monika_namebox.png");
	game.create_character("Yuri", "???", "img/characters/yuri.png", Color::from_rgba8(184, 168, 255), "img/gui/yuri_textbox.png");
	game.create_character("Natsuki", "???", "img/characters/natsuki.png", Color::from_rgba8(255, 203, 228), "img/gui/natsuki_textbox.png", "img/gui/natsuki_namebox.png");

	//game.autofocus_enable();
	//game.autozorder_enable();
	//game.add_new_dialogue("...0");
	game.show_character("Yuri", "t41", 1);
	game.show_character("Sayori", "t42", 0);
	game.show_character("Monika", "t43", 2);
	game.show_character("Natsuki", "t44", 1);
	game.add_new_dialogue("...1");
	game.move_textbox("top");
	game.play_sound("sounds/fc_starting.ogg", 0, 2000, 3000, 10);
	game.add_new_dialogue("...2");
	game.move_textbox("bottomleft");
	game.add_new_dialogue("Y a personne ??");
	game.move_textbox("bottom");
	game.play_sound("sounds/new_hour.ogg", 0, 2000, 3000);
	game.add_new_dialogue("...3");
	game.play_music("sounds/2.ogg", 0, 1000, 1.0); 
	game.show_character("Sayori", "t11");
	game.rename_character("Sayori", "xxxxx");
	game.add_new_dialogue("Sayori", "HEYYYYYY!");
	game.change_textbox("Sayori", "img/gui/yuri_textbox.png");
	game.change_namebox("Sayori", "img/gui/monika_namebox.png");
	game.change_namebox_text_color("Sayori", Color::from_rgba8(0, 0, 0));
	game.rename_character("Sayori", "yyyyy"); 
	game.add_new_dialogue("Sayori", "Hihihi!");
	game.add_new_dialogue("I say that, but joining Sayori's club might be good because given the enthusiasm she has when she talks about it, it has to be nice.");
	game.show_background("img/backgrounds/class.png");
	game.autofocus_enable();
	game.autozorder_enable();
	game.play_music("sounds/3.ogg", 2, 1000); 
	game.show_character("Yuri", "t41");
	game.show_character("Sayori", "t42");
	game.show_character("Monika", "t43");
	game.show_character("Natsuki", "t44");
	game.add_new_dialogue("Elle est chiante...");
	color::str::add_new_string_color("dirt", Color::from_rgba8(50, 20, 30));
	game.show_background(Color::from_string("dirt"));
	game.add_new_dialogue("Yuri", "How are you MC?");
	game.stop_music(2000); 
	game.rename_character("Yuri", "Yuri beta");
	game.hide_background();
	game.add_new_dialogue("Yuri", "...");
	game.show_background("img/backgrounds/corridor.png");
	game.show_character("Sayori", "t42"); 
	game.show_character("Natsuki", 50);
	game.add_new_dialogue("Natsuki", "HEYYYYYY!");
	game.add_new_dialogue("...");
	//game.autozorder_enable();
	game.add_new_dialogue("Sayori", "How are you MC?");

	game.run();

	return 0;
}