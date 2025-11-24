#include "transform.h"
#include "image.h"

#include <iostream>

Transform::Transform()
	: transform_finished_(false), step_finished_(false)
{

}

void Transform::show(Image& image)
{
	//std::cout << "SHOW ***********************************************************\n";
	image.alpha_ = 255;
	image.texture_->set_alpha_mod(image.alpha_);
}

void Transform::hide(Image& image)
{
	//std::cout << "HIDE ***********************************************************\n";
	image.alpha_ = 0;
	image.texture_->set_alpha_mod(image.alpha_);
}

void Transform::rotate(Image& image, const double angle)
{
	image.angle_ = angle;
}

void Transform::flip_vertically(Image& image)
{
	//std::cout << "FLIP V ***********************************************************\n";
	image.flip_ = SDL_FLIP_VERTICAL;
}

void Transform::flip_horizontally(Image& image)
{
	//std::cout << "FLIP H ***********************************************************\n";
	image.flip_ = SDL_FLIP_HORIZONTAL;
}

void Transform::flip_normal(Image& image)
{
	//std::cout << "FLIP N ***********************************************************\n";
	image.flip_ = SDL_FLIP_NONE;
}

int Transform::get_xcenter(Image& image) const
{
	//std::cout << "GET XCENTER ***********************************************************\n";
	return image.position_.w / 2;
}

void Transform::resize(Image& image, const int w, const int h)
{
	//std::cout << "RESIZE ***********************************************************\n";
	image.position_ = {image.position_.x, image.position_.y, w, h};
}

void Transform::set_position(Image& image, const int x, const int y, Uint64 time)
{
	//std::cout << "SET POS ***********************************************************\n";
	image.position_ = {x, y, image.position_.w, image.position_.h};
}

void Transform::set_position_xcenter(Image& image, const int x, const int y)
{
	//std::cout << "SET POS XCENTER ***********************************************************\n";
	image.position_ = {x - std::abs(get_xcenter(image)), y, image.position_.w, image.position_.h};
}

void Transform::zoom(Image& image, const float zoom, Uint64 time)
{
	float position_w = float(image.position_.w);
	float position_h = float(image.position_.h);

	if(time == 0)
	{
		position_w *= zoom;
		position_h *= zoom;

		image.position_.w = int(position_w);
		image.position_.h = int(position_h);

		return;
	}

	static float converging_zoom = 1.0f; //will go slowly to the value of the "zoom" parameter

	if(!transform_finished_ && zoom != 1.0f)
	{
		reset(image);

		position_w = float(image.position_.w) * converging_zoom;
		position_h = float(image.position_.h) * converging_zoom;

		image.position_.w = int(position_w);
		image.position_.h = int(position_h);

		converging_zoom += (zoom - 1.0f) / (60.0f / (1000.0f / float(time))); //1.0f = zoom sans modif, 60.0f = FPS, 1000.0f = 1000 ms

		if((zoom < 1.0f && converging_zoom <= zoom) || (zoom > 1.0f && converging_zoom >= zoom))
		{
			transform_finished_ = true;
		}
	}
}

void Transform::set_center(Image& image, Uint64 time)
{
	static float delta_x = (1280.0f / 2.0f - std::abs(get_xcenter(image))) - image.position_.x; //1280 = SCREEN_WIDTH
	static int init_position_x = image.position_.x;
	static float delta_x_frame = 0.0f;

	float position_x = float(image.position_.x);

	//std::cout << "POSITION_X : " << position_x << std::endl;
	//std::cout << "DELTA_X : " << delta_x << std::endl;

	if(!transform_finished_ && time != 0)
	{
		reset(image);

		//TODO : ne marche pas si init_position_x est négatif
		delta_x_frame += delta_x / (60.0f / (1000.0f / float(time))); //TODO : = ne marche que si init_position_x > 1280/2
		position_x = /* += */ delta_x_frame; //TODO : += ne marche que si init_position_x > 1280/2

		image.position_.x = int(position_x);

		//std::cout << "POSITION_X : " << image.position_.x << std::endl;
		//std::cout << "DELTA X FRAME : " << delta_x_frame << std::endl;

		//std::cout << init_position_x << " et " << image.position_.x  << " et " << init_position_x + delta_x << std::endl;

		if((init_position_x < 1280/2 && image.position_.x >= init_position_x + delta_x)
		|| (init_position_x > 1280/2 && image.position_.x <= init_position_x + delta_x)) //TODO
		{
			transform_finished_ = true;
		}
	}
}

void Transform::night_filter(Image& image)
{
	//std::cout << "N FILTER ***********************************************************\n";
	image.r_ = 127;
	image.g_ = 127;
	image.b_ = 165;
	image.texture_->set_color_mod(image.r_, image.g_, image.b_);
}

void Transform::afternoon_filter(Image& image)
{
	//std::cout << "A FILTER ***********************************************************\n";
	image.r_ = 210;
	image.g_ = 150;
	image.b_ = 130;
	image.texture_->set_color_mod(image.r_, image.g_, image.b_);
}

void Transform::own_filter(Image& image, const Uint8 r, const Uint8 g, const Uint8 b)
{
	//std::cout << "O FILTER ***********************************************************\n";
	image.r_ = r;
	image.g_ = g;
	image.b_ = b;
	image.texture_->set_color_mod(image.r_, image.g_, image.b_);
}

void Transform::reset(Image& image)
{
	image.position_ = image.initial_rect_;
}

void Transform::t11(Image& image, Uint64 time_step)
{
	//Le jeu tourne à 60 FPS

	//TODO : étape 1 => ne gérer qu'une seule animation par ligne pour l'instant

	//Compteur partagé pour les deux lignes
	//static Uint64 timeout = SDL_GetTicks64() + 10000; //ms
	
	//zoom(image, 0.5f, 3000); //dézoom de moitié sur 3 secondes
	set_center(image, 5000); //déplacement vers le center sur 5 secondes

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