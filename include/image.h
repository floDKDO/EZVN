#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Image
{
	public:
		Image(const std::string path, const int x, const int y, SDL_Renderer* renderer, const int zorder=0);
		~Image();
		Image(const Image& i);
		Image& operator=(Image i);

		friend void swap(Image& i1, Image& i2)
		{
			using std::swap; 

			swap(i1.zorder, i2.zorder);
			swap(i1.position, i2.position);
			swap(i1.name, i2.name);
			swap(i1.path, i2.path);
			swap(i1.alpha, i2.alpha);
			swap(i1.angle, i2.angle);
			swap(i1.flip, i2.flip);
			swap(i1.r, i2.r); swap(i1.g, i2.g); swap(i1.b, i2.b);
			swap(i1.frame_index, i2.frame_index);
			swap(i1.is_gif, i2.is_gif);
			swap(i1.gif, i2.gif);
			swap(i1.texture, i2.texture);
			swap(i1.renderer, i2.renderer);
		}

		void show();
		void hide();

		void flip_vertically();
		void flip_horizontally();
		void flip_normal();

		void resize(const int w, const int h);
		void set_position(const int x, const int y);

		//TODO : void set_character_position(Transform t);

		void night_filter();
		void afternoon_filter();
		void own_filter(const Uint8 r, const Uint8 g, const Uint8 b);

		void draw(SDL_Renderer* renderer);

		int zorder;
		SDL_Rect position;

	protected:
		

	private:
		std::string name; //TODO : les deux attributs ont la même valeur...
		std::string path;
		
		Uint8 alpha;
		double angle;
		
		SDL_RendererFlip flip;
		Uint8 r, g, b;

		int frame_index;
		bool is_gif;
		IMG_Animation* gif;

		SDL_Texture* texture;
		SDL_Renderer* renderer; //solution temporaire => cette classe ne possède pas / don't owns ce pointeur
};