#include "transform.h"

#include <iostream>

Transform::Transform()
{

}

void Transform::reset(Character& character)
{
	character.character_.position_ = character.initial_rect_; //RESET
}

//TODO : dans update()
void Transform::t11(Character& character, Uint64 time_step)
{
	//Le jeu tourne à 60 FPS

	//TODO : étape 1 => ne gérer qu'une seule animation par ligne pour l'instant

	//Compteur partagé pour les deux lignes
	static Uint64 timeout = SDL_GetTicks64() + 1000; //ms

	if(SDL_GetTicks64() <= timeout)
	{
		reset(character); //TODO : à appeler dans zoom()
		character.character_.zoom(1.5f, 1000);
	}

	/*if(SDL_GetTicks64() > timeout)
	{
		character.character_.set_center(1000);
	}*/
	//en 0.5 s

	//Passer ici uniquement lorsque les 0.5 s sont passées <=> SDL_GetTicks64() >= timeout

	//Compteur propre à la ligne
	//character.character_.set_position(character.character_.position_.x, character.character_.position_.y - 26);
	//en 1.0 s
}