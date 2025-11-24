#pragma once

#include "image.h"

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

		void show(Image& image);
		void hide(Image& image);

		void rotate(Image& image, const double angle);

		void flip_vertically(Image& image);
		void flip_horizontally(Image& image);
		void flip_normal(Image& image);

		int get_xcenter(Image& image) const;
		void zoom(Image& image, const float zoom, Uint64 time = 0);
		void resize(Image& image, const int w, const int h);
		void set_position(Image& image, const int x, const int y, Uint64 time = 0);
		void set_position_xcenter(Image& image, const int x, const int y);
		void set_center(Image& image, Uint64 time = 0);

		//TODO : void set_character_position(Transform t);

		void night_filter(Image& image);
		void afternoon_filter(Image& image);
		void own_filter(Image& image, const Uint8 r, const Uint8 g, const Uint8 b);

		void reset(Image& image);
		void t11(Image& image, Uint64 time_step);

		bool transform_finished_;
		bool step_finished_;
};

