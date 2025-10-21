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
		Image& operator=(const Image& i);

		void show();
		void hide();

		void flip_vertically();
		void flip_horizontally();
		void flip_normal();

		void resize(const int w, const int h);
		void set_position(const int x, const int y);

		void night_filter();
		void afternoon_filter();
		void own_filter(const Uint8 r, const Uint8 g, const Uint8 b);

		void draw(SDL_Renderer* renderer);

		int zorder;
		SDL_Rect position;

	protected:
		

	private:
		std::string name;
		
		Uint8 alpha;
		double angle;
		
		SDL_RendererFlip flip;
		Uint8 r, g, b;

		int frame_index;
		bool is_gif;
		IMG_Animation* gif;

		SDL_Texture* texture;
		SDL_Renderer* renderer; //solution temporaire
};