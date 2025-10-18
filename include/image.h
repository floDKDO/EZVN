#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

class Image
{
	public:
		Image();
		Image(std::string path, SDL_Renderer* renderer);
		Image(std::string path, SDL_Renderer* renderer, int zorder);
		~Image();
		void show();
		void hide();

		void flip_vertically();
		void flip_horizontally();
		void flip_normal();

		void resize(int w, int h);
		void position(int x, int y);

		void night_filter();
		void afternoon_filter();
		void own_filter(int r, int g, int b);

		void draw(SDL_Renderer* renderer);

		std::string name;
		SDL_Rect rect;
		int alpha;
		double angle;
		int zorder;
		SDL_RendererFlip flip;
		int r, g, b;

		bool is_gif;
		int frame_number;
		IMG_Animation* gif;

		SDL_Texture* texture;

	protected:
		

	private:
		
};

