#pragma once

#include "character.h"

class Transform
{
	/* TODO : 
	* on peut modifier : 
	* zoom
	* position x y
	* timer pour chaque modif
	* on show/replace/hide
	*/
	public:
		Transform();

		void reset(Character& character);
		void t11(Character& character, Uint64 time_step);
};

