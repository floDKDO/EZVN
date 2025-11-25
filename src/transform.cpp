#include "transform.h"
#include "image.h"

#include <iostream>

//TODO : créer une sous classe de "TransformStep" que la Transform va jouer une par une dans l'ordre (=> passer à la suivante quand transform_step_finished_ de la TransformStep courante est à true)
//Quand toutes les TransformStep seront terminées, alors mettre transform_finished_ à true

//TODO : beaucoup trop de répétitions dans les méthodes de cette classe

Transform::Transform()
	: transform_finished_(false)
{

}

void Transform::show(Image& image, Uint64 time)
{
	if(time == 0 && !transform_finished_)
	{
		image.alpha_ = 255;
		image.texture_->set_alpha_mod(image.alpha_);
		transform_finished_ = true;
		return;
	}

	static float delta_alpha = 255 - image.alpha_;
	static int init_alpha = image.alpha_;
	static float delta_alpha_frame = 0.0f;
	static float image_alpha = float(image.alpha_);

	if(!transform_finished_ && time != 0)
	{
		reset(image);

		delta_alpha_frame = delta_alpha / (60.0f / (1000.0f / float(time)));
		image_alpha += delta_alpha_frame;

		image.alpha_ = int(image_alpha);
		image.texture_->set_alpha_mod(image.alpha_);

		if(init_alpha < 255 && image.alpha_ >= 255)
		{
			transform_finished_ = true;
		}
	}
}

void Transform::hide(Image& image, Uint64 time)
{
	if(time == 0 && !transform_finished_)
	{
		image.alpha_ = 0;
		image.texture_->set_alpha_mod(image.alpha_);
		transform_finished_ = true;
		return;
	}

	static float delta_alpha = 0 - image.alpha_;
	static int init_alpha = image.alpha_;
	static float delta_alpha_frame = 0.0f;
	static float image_alpha = float(image.alpha_);

	if(!transform_finished_ && time != 0)
	{
		reset(image);

		delta_alpha_frame = delta_alpha / (60.0f / (1000.0f / float(time)));
		image_alpha += delta_alpha_frame;

		image.alpha_ = int(image_alpha);
		image.texture_->set_alpha_mod(image.alpha_);

		if(init_alpha > 0 && image.alpha_ <= 0)
		{
			transform_finished_ = true;
		}
	}
}

void Transform::rotate(Image& image, const double angle, Uint64 time)
{
	if(time == 0 && !transform_finished_)
	{
		image.angle_ = angle;
		transform_finished_ = true;
		return;
	}

	static float delta_angle = angle - image.angle_;
	static int init_angle = image.angle_;
	static float delta_angle_frame = 0.0f;
	static float image_angle = float(image.angle_);

	if(!transform_finished_ && time != 0)
	{
		reset(image);

		delta_angle_frame = delta_angle / (60.0f / (1000.0f / float(time)));
		image_angle += delta_angle_frame;

		image.angle_ = int(image_angle);

		if((init_angle < angle && image.angle_ >= angle) || (init_angle > angle && image.angle_ <= angle))
		{
			transform_finished_ = true;
		}
	}
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

void Transform::resize(Image& image, const int w, const int h, Uint64 time)
{
	if(time == 0 && !transform_finished_)
	{
		image.position_ = {image.position_.x, image.position_.y, w, h};
		transform_finished_ = true;
		return;
	}

	static float delta_w = w - image.position_.w;
	static float delta_h = h - image.position_.h;

	static int init_position_w = image.position_.w;
	static float delta_w_frame = 0.0f;
	static float position_w = float(image.position_.w);

	static int init_position_h = image.position_.h;
	static float delta_h_frame = 0.0f;
	static float position_h = float(image.position_.h);

	if(!transform_finished_ && time != 0)
	{
		reset(image);

		delta_w_frame = delta_w / (60.0f / (1000.0f / float(time)));
		position_w += delta_w_frame;

		delta_h_frame = delta_h / (60.0f / (1000.0f / float(time)));
		position_h += delta_h_frame;

		image.position_.w = int(position_w);
		image.position_.h = int(position_h);

		if(((init_position_w < w && image.position_.w >= w) || (init_position_w > w && image.position_.w <= w))
		&& ((init_position_h < h && image.position_.h >= h) || (init_position_h > h && image.position_.h <= h)))
		{
			transform_finished_ = true;
		}
	}
}

void Transform::set_position(Image& image, const int x, const int y, Uint64 time)
{
	if(time == 0 && !transform_finished_)
	{
		image.position_ = {x, y, image.position_.w, image.position_.h};
		transform_finished_ = true;
		return;
	}

	static float delta_x = x - image.position_.x;
	static float delta_y = y - image.position_.y;

	static int init_position_x = image.position_.x;
	static float delta_x_frame = 0.0f;
	static float position_x = float(image.position_.x);

	static int init_position_y = image.position_.y;
	static float delta_y_frame = 0.0f;
	static float position_y = float(image.position_.y);

	if(!transform_finished_ && time != 0)
	{
		reset(image);

		delta_x_frame = delta_x / (60.0f / (1000.0f / float(time)));
		position_x += delta_x_frame;

		delta_y_frame = delta_y / (60.0f / (1000.0f / float(time)));
		position_y += delta_y_frame;

		image.position_.x = int(position_x);
		image.position_.y = int(position_y);

		if(((init_position_x < x && image.position_.x >= x) || (init_position_x > x && image.position_.x <= x))
		&& ((init_position_y < y && image.position_.y >= y) || (init_position_y > y && image.position_.y <= y)))
		{
			transform_finished_ = true;
		}
	}
}

void Transform::set_position_xcenter(Image& image, const int x, const int y, Uint64 time)
{
	if(time == 0 && !transform_finished_)
	{
		image.position_ = {x - std::abs(get_xcenter(image)), y, image.position_.w, image.position_.h};
		transform_finished_ = true;
		return;
	}

	static float delta_x = (x - std::abs(get_xcenter(image))) - image.position_.x; 
	static float delta_y = y - image.position_.y;

	static int init_position_x = image.position_.x;
	static float delta_x_frame = 0.0f;
	static float position_x = float(image.position_.x);

	static int init_position_y = image.position_.y;
	static float delta_y_frame = 0.0f;
	static float position_y = float(image.position_.y);

	if(!transform_finished_ && time != 0)
	{
		reset(image);

		delta_x_frame = delta_x / (60.0f / (1000.0f / float(time)));
		position_x += delta_x_frame;

		delta_y_frame = delta_y / (60.0f / (1000.0f / float(time)));
		position_y += delta_y_frame;

		image.position_.x = int(position_x);
		image.position_.y = int(position_y);

		if(((init_position_x < x && image.position_.x >= x) || (init_position_x > x && image.position_.x <= x))
		&& ((init_position_y < y && image.position_.y >= y) || (init_position_y > y && image.position_.y <= y))) 
		{
			transform_finished_ = true;
		}
	}
}

void Transform::zoom(Image& image, const float zoom, Uint64 time)
{
	float position_w = float(image.position_.w);
	float position_h = float(image.position_.h);

	if(time == 0 && !transform_finished_)
	{
		position_w *= zoom;
		position_h *= zoom;

		image.position_.w = int(position_w);
		image.position_.h = int(position_h);

		transform_finished_ = true;

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
	if(time == 0 && !transform_finished_)
	{
		image.position_ = {1280 / 2 - std::abs(get_xcenter(image)), image.position_.y, image.position_.w, image.position_.h};
		transform_finished_ = true;
		return;
	}

	static float delta_x = (1280.0f / 2.0f - std::abs(get_xcenter(image))) - image.position_.x; //1280 = SCREEN_WIDTH
	static int init_position_x = image.position_.x;
	static float delta_x_frame = 0.0f;
	static float position_x = float(image.position_.x);

	if(!transform_finished_ && time != 0)
	{
		reset(image);

		delta_x_frame = delta_x / (60.0f / (1000.0f / float(time))); 
		position_x += delta_x_frame; 

		image.position_.x = int(position_x);

		if((init_position_x < 1280/2 && image.position_.x >= init_position_x + delta_x)
		|| (init_position_x > 1280/2 && image.position_.x <= init_position_x + delta_x)) 
		{
			transform_finished_ = true;
		}
	}
}

void Transform::night_filter(Image& image, Uint64 time)
{
	if(time == 0 && !transform_finished_)
	{
		image.r_ = 127;
		image.g_ = 127;
		image.b_ = 165;
		image.texture_->set_color_mod(image.r_, image.g_, image.b_);
		transform_finished_ = true;
		return;
	}

	static float delta_r = 127 - image.r_;
	static float delta_g = 127 - image.g_;
	static float delta_b = 165 - image.b_;

	static int init_image_r = image.r_;
	static float delta_r_frame = 0.0f;
	static float image_r = float(image.r_);

	static int init_image_g = image.g_;
	static float delta_g_frame = 0.0f;
	static float image_g = float(image.g_);

	static int init_image_b = image.b_;
	static float delta_b_frame = 0.0f;
	static float image_b = float(image.b_);

	if(!transform_finished_ && time != 0)
	{
		reset(image);

		delta_r_frame = delta_r / (60.0f / (1000.0f / float(time)));
		image_r += delta_r_frame;

		delta_g_frame = delta_g / (60.0f / (1000.0f / float(time)));
		image_g += delta_g_frame;

		delta_b_frame = delta_b / (60.0f / (1000.0f / float(time)));
		image_b += delta_b_frame;

		image.r_ = int(image_r);
		image.g_ = int(image_g);
		image.b_ = int(image_b);

		image.texture_->set_color_mod(image.r_, image.g_, image.b_);

		if(((init_image_r < 127 && image.r_ >= 127) || (init_image_r > 127 && image.r_ <= 127))
		&& ((init_image_g < 127 && image.g_ >= 127) || (init_image_g > 127 && image.g_ <= 127))
		&& ((init_image_b < 165 && image.b_ >= 165) || (init_image_b > 165 && image.b_ <= 165)))
		{
			transform_finished_ = true;
		}
	}
}

void Transform::afternoon_filter(Image& image, Uint64 time)
{
	if(time == 0 && !transform_finished_)
	{
		image.r_ = 210;
		image.g_ = 150;
		image.b_ = 130;
		image.texture_->set_color_mod(image.r_, image.g_, image.b_);
		transform_finished_ = true;
		return;
	}

	static float delta_r = 210 - image.r_;
	static float delta_g = 150 - image.g_;
	static float delta_b = 130 - image.b_;

	static int init_image_r = image.r_;
	static float delta_r_frame = 0.0f;
	static float image_r = float(image.r_);

	static int init_image_g = image.g_;
	static float delta_g_frame = 0.0f;
	static float image_g = float(image.g_);

	static int init_image_b = image.b_;
	static float delta_b_frame = 0.0f;
	static float image_b = float(image.b_);

	if(!transform_finished_ && time != 0)
	{
		reset(image);

		delta_r_frame = delta_r / (60.0f / (1000.0f / float(time)));
		image_r += delta_r_frame;

		delta_g_frame = delta_g / (60.0f / (1000.0f / float(time)));
		image_g += delta_g_frame;

		delta_b_frame = delta_b / (60.0f / (1000.0f / float(time)));
		image_b += delta_b_frame;

		image.r_ = int(image_r);
		image.g_ = int(image_g);
		image.b_ = int(image_b);

		image.texture_->set_color_mod(image.r_, image.g_, image.b_);


		if(((init_image_r < 210 && image.r_ >= 210) || (init_image_r > 210 && image.r_ <= 210))
		&& ((init_image_g < 150 && image.g_ >= 150) || (init_image_g > 150 && image.g_ <= 150))
		&& ((init_image_b < 130 && image.b_ >= 130) || (init_image_b > 130 && image.b_ <= 130)))
		{
			transform_finished_ = true;
		}
	}
}

void Transform::own_filter(Image& image, const Uint8 r, const Uint8 g, const Uint8 b, Uint64 time)
{
	if(time == 0 && !transform_finished_)
	{
		image.r_ = r;
		image.g_ = g;
		image.b_ = b;
		image.texture_->set_color_mod(image.r_, image.g_, image.b_);
		transform_finished_ = true;
		return;
	}

	static float delta_r = r - image.r_;
	static float delta_g = g - image.g_;
	static float delta_b = b - image.b_;

	static int init_image_r = image.r_;
	static float delta_r_frame = 0.0f;
	static float image_r = float(image.r_);

	static int init_image_g = image.g_;
	static float delta_g_frame = 0.0f;
	static float image_g = float(image.g_);

	static int init_image_b = image.b_;
	static float delta_b_frame = 0.0f;
	static float image_b = float(image.b_);

	if(!transform_finished_ && time != 0)
	{
		reset(image);

		delta_r_frame = delta_r / (60.0f / (1000.0f / float(time)));
		image_r += delta_r_frame;

		delta_g_frame = delta_g / (60.0f / (1000.0f / float(time)));
		image_g += delta_g_frame;

		delta_b_frame = delta_b / (60.0f / (1000.0f / float(time)));
		image_b += delta_b_frame;

		image.r_ = int(image_r);
		image.g_ = int(image_g);
		image.b_ = int(image_b);

		image.texture_->set_color_mod(image.r_, image.g_, image.b_);

		if(((init_image_r < r && image.r_ >= r) || (init_image_r > r && image.r_ <= r))
		&& ((init_image_g < g && image.g_ >= g) || (init_image_g > g && image.g_ <= g))
		&& ((init_image_b < b && image.b_ >= b) || (init_image_b > b && image.b_ <= b)))
		{
			transform_finished_ = true;
		}
	}
}

//TODO : aussi sauvegarder angle, alpha etc ??
//TODO : si pas besoin, alors utilité d'appeler cette fonction dans les méthodes au-dessus ??
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
	
	//TODO : transfrorm_finished_ va bloquer les autres Transforms
	//TODO : faire en sorte que plusieurs Transforms peuvent se réaliser de manière concurrente
	//zoom(image, 0.5f, 0); //dézoom de moitié sur 3 secondes
	//set_center(image, 5000); //déplacement vers le centre sur 5 secondes

	//set_position_xcenter(image, 720, 100, 3000);

	//set_position(image, -100, -200, 3000);

	//resize(image, 480, 480, 3000);

	//rotate(image, 360, 3000);

	//show(image, 3000);

	//hide(image, 3000);

	//night_filter(image, 3000);

	//afternoon_filter(image, 3000);

	//own_filter(image, 254, 10, 30, 3000);

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