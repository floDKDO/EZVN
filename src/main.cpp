#include "game.h"

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) //mandatory to use argc and argv for SDL2
{
	//Step 0 : create a Game object
	Game game;


	//First step : character creation
	game.create_character("Sayori", "???", Color::from_rgba8(168, 224, 255), "resources/img/gui/sayori_textbox.png", "resources/img/gui/sayori_namebox.png");
	game.create_character("Monika", "???", Color::from_rgba8(168, 255, 169), "resources/img/gui/monika_textbox.png", "resources/img/gui/monika_namebox.png");
	game.create_character("Yuri", "???", Color::from_rgba8(184, 168, 255), "resources/img/gui/yuri_textbox.png", "resources/img/gui/yuri_namebox.png");
	game.create_character("Natsuki", "???", Color::from_rgba8(255, 203, 228), "resources/img/gui/natsuki_textbox.png", "resources/img/gui/natsuki_namebox.png");
	game.create_character("MC", "MC");

	game.create_composite_image("Sayori", "4x", 960, 960, {
		ImageInfo("resources/img/characters/sayori_turned_uniform_left_up.png", 0, 0),
		ImageInfo("resources/img/characters/sayori_turned_uniform_right_up.png", 0, 0),
		ImageInfo("resources/img/characters/x.png", 0, 0)
	});

	game.create_composite_image("Monika", "3b", 960, 960, {
		ImageInfo("resources/img/characters/monika_forward_uniform_left_waist.png", 0, 0),
		ImageInfo("resources/img/characters/monika_forward_uniform_right_down.png", 0, 0),
		ImageInfo("resources/img/characters/b.png", 0, 0)
	});

	game.create_composite_image("Monika", "2b", 960, 960, {
		ImageInfo("resources/img/characters/monika_forward_uniform_left_down.png", 0, 0),
		ImageInfo("resources/img/characters/monika_forward_uniform_right_waist.png", 0, 0),
		ImageInfo("resources/img/characters/b.png", 0, 0)
	});

	game.create_composite_image("Yuri", "3j", 960, 960, {
		ImageInfo("resources/img/characters/yuri_turned_uniform_left_up.png", 0, 0),
		ImageInfo("resources/img/characters/yuri_turned_uniform_right_up.png", 0, 0),
		ImageInfo("resources/img/characters/j.png", 0, 0)
	});

	game.create_composite_image("Natsuki", "5c", 960, 960, {
		ImageInfo("resources/img/characters/natsuki_crossed(ff)_uniform.png", 0, 0),
		ImageInfo("resources/img/characters/c.png", 18, 22)
	});


	//Second step : write your own story!
	game.autofocus_enable();
	game.autozorder_enable();
	game.show_background("resources/img/backgrounds/cafeteria.png", "fade");
	game.play_sound("resources/sounds/school_bell.ogg");
	game.show_dialogue("I am feeling quite lonely right now...");
	game.show_dialogue("I know, I should try to find Sayori!");
	game.play_music("resources/sounds/2.ogg");
	game.show_character({"Sayori", "4x"}, "t11");
	game.show_dialogue("Sayori", "No need, I'm already there!");
	game.show_dialogue("Sayori!");
	game.show_dialogue("She almost scared me...");
	game.rename_character("Sayori", "Sayori");
	game.show_dialogue("Sayori", "Come to my club! You will feel less lonely there.");
	game.show_character("MC"); //mandatory for now even if the MC has no image
	game.show_dialogue("MC", "If you say so...");
	game.hide_character("Sayori");
	game.show_background("resources/img/backgrounds/corridor.png", {"fade", 500, 500, 500});
	game.show_dialogue("I am following Sayori to her club not without feeling nervous.");
	game.show_dialogue("I heard that her club is full of girls!");
	game.show_character({"Sayori", "4x"}, "t11");
	game.show_dialogue("Sayori", "We arrived!");
	game.show_dialogue("Sayori", "Before entering, I have a question MC.");
	game.show_dialogue("MC", "I'm listening.");
	game.show_dialogue("Sayori", "What is your favorite color?");
	game.change_textbox("Sayori", "resources/img/gui/monika_textbox.png");
	game.change_namebox("Sayori", "resources/img/gui/monika_namebox.png");
	game.change_namebox_text_color("Sayori", Color::from_rgba8(168, 255, 169));
	game.move_textbox("bottomleft");
	game.show_dialogue("Sayori", "Green?");
	game.change_textbox("Sayori", "resources/img/gui/yuri_textbox.png");
	game.change_namebox("Sayori", "resources/img/gui/yuri_namebox.png");
	game.change_namebox_text_color("Sayori", Color::from_rgba8(184, 168, 255));
	game.move_textbox("topright");
	game.show_dialogue("Sayori", "Purple?");
	game.change_textbox("Sayori", "resources/img/gui/natsuki_textbox.png");
	game.change_namebox("Sayori", "resources/img/gui/natsuki_namebox.png");
	game.change_namebox_text_color("Sayori", Color::from_rgba8(255, 203, 228));
	game.move_textbox("bottomright");
	game.show_dialogue("Sayori", "Pink?");
	game.change_textbox("Sayori", "resources/img/gui/sayori_textbox.png");
	game.change_namebox("Sayori", "resources/img/gui/sayori_namebox.png");
	game.change_namebox_text_color("Sayori", Color::from_rgba8(168, 224, 255));
	game.move_textbox("bottom");
	game.show_dialogue("Sayori", "Or blue?");
	//game.show_dialogue("MC", "I...");
	game.show_choice_menu("MC", "I...", { //pas encore complètement fonctionnel
		{"Green", 
			{"MC", "I choose green!"}
		}, 
		{"Purple", 
			{"MC", "I choose purple!"}
		}, 
		{"Pink", 
			{"MC", "I choose pink!"}
		}, 
		{"Blue", 
			{"MC", "I choose blue!"}
		},
		{"Heu...",
			{"I don't know..."}
		}
	}); 
	game.play_sound("resources/sounds/door_open.ogg");
	game.show_dialogue("Sayori", "Follow me!");
	game.show_character("Sayori", "lhide");
	game.show_background("resources/img/backgrounds/club.png", "fade");
	game.stop_sound();
	game.play_music("resources/sounds/3.ogg", 0, 500);
	game.show_character({"Yuri", "3j"}, "t41", 1);
	game.show_character({"Sayori", "4x"}, "h42", 0);
	game.show_character({"Monika", "3b"}, "t43", 2);
	game.show_character({"Natsuki", "5c"}, "t44", 1);
	game.show_dialogue("Sayori", "Look everyone, I brought a new member.");
	game.show_dialogue("Monika", "Hi there!");
	game.show_character({"Monika", "2b"}); 
	game.show_dialogue("Yuri", "Hello, nice to meet you");
	game.show_dialogue("Natsuki", "Hey.");
	game.show_dialogue("Gulp...");


	//Third and last step : run the game
	game.run();

	return 0;
}

//TODO : les paramètres "inutiles" des fonctions de la SDL pourraient être préremplis (valeur fixée pour ces paramètres) => ex : les 2 premiers paramètres de sdl::Texture::query()
// idem pour les valeurs fixées (ex : -1 pour tous les channels) => rendre ça plus intuitif

//TODO : utiliser std::clamp pour s'assurer qu'une valeur soit dans un intervalle

//TODO : pour la collision entre un SDL_Rect et la souris, utiliser SDL_PointInRect()

//TODO : quand une méthode prend un pointeur en paramètre (ex : UiWidget*), vérifier que celui-ci ne vaut pas nullptr

//TODO : créer des variables dans des if si utile

//TODO : sûrement remplacer les paramètres std::vector par std::initializer_list quand c'est possible

//TODO : utiliser les bons types (ex : unsigned int, std::string_view...)

//TODO:  être cohérent entre utiliser les membres ou les paramètres du constructeur (ex : renderer ou renderer_)

//TODO : nettoyer le code => retirer les includes inutiles (attention que je n'inclus pas le même header dans un .h et son .cpp associé + utiliser des forward declarations quand c'est possible), 
//                        => retirer les membres et commentaires inutiles, utiliser using pour simplifier les types etc.

//TODO : composition over inheritance

//TODO : éventuellement remplacer les C-casts par static_cast

//TODO : si un membre d'une classe est retourné, je peux retourner une référence

//TODO : crash si aucun dialogue => current_script_index_ a la valeur initiale -1 ??

//TODO : mettre des constructeurs par défaut dans les structs pour simplifier l'initialisation de leurs membres 

//TODO : créer des variables pour les éléments de tableau utilisés à plusieurs endroits dans une même méthode

//TODO : lambdas à la place des std::bind ??

//TODO : passer les vector par ref ??

//TODO : système de logs

//TODO : si valeur de wrap_length pas connue dans le constructeur ?

//TODO : si je garde la scrollbar, créer une classe intermédiaire (ex : Scrollable) pour ne pas répéter le code du Slider

//TODO : membre static => static inline ou static constexpr pour attribuer une valeur directement dans le .h

//TODO : utiliser des free functions si possible

//TODO : utiliser friend (méthode membre, non membre ou classe) si nécessaire

//TODO : anonymous namespace au lieu de static pour des fonctions static et des variables globales static propres à une translation unit

//TODO : peu de lisibilité avec tous les paramètres par défaut

//TODO : si trop de code en commun entre Sound et Music, pk pas créer une classe de base Audio ??

//TODO : dans la member initialization list, ne pas écrire : transform_(Transform(transform_name)) mais transform_(transform_name)

//TODO : il ne faudrait pas utiliser std::string_view en tant que clef dans une std::map ou std::unordered_map

//TODO : ajouter d'autres options utiles dans le menu Settings ?

//TODO : améliorer le système de UI pour qu'un UI puisse contenir plusieurs UI membres

//TODO : inputfield qui demande le nom du joueur au premier lancement du jeu => nécessite les variables persistantes

//TODO : pause et window hide, pouvoir attendre la fin exacte d'un son

//TODO : la textbox noire apparaît brièvement pour le premier dialogue, de même que des anciens noms dans la namebox

//TODO : il faudrait qu'il y ait une erreur si : 
	//- plusieurs personnages ont la même character_variable
	//- un personnage possède une character_variable vide (= "") car il serait considéré à tort comme le narrateur

//si un son sur le même channel est lancé alors qu'un autre son était en train de se jouer, il faut stopper ce dernier
//deux sons sur un channel différent ne s'interfèrent pas
//fadein = facile
//fadeout => fadeout un précédent son uniquement s'il est sur le même channel

//play music => Fadeout gives the fadeout time for currently playing music, in seconds, while fadein gives the time it takes to fade in the new music.
//stop music => fadeout