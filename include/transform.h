#pragma once

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

		void t11(int& x, int& y);

		//Prend des paramètres qui seront modifiés par la fonction => références
		//Ex : t11(int& x, int& y);
};

