#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

class Image
{
	public:
		Image();
		Image(const std::string path, SDL_Renderer* renderer);
		Image(const std::string path, SDL_Renderer* renderer, const int zorder);
		~Image();
		void show();
		void hide();

		void flip_vertically();
		void flip_horizontally();
		void flip_normal();

		void resize(const int w, const int h);
		void position(const int x, const int y);

		void night_filter();
		void afternoon_filter();
		void own_filter(const Uint8 r, const Uint8 g, const Uint8 b);

		void draw(SDL_Renderer* renderer);

		std::string name;
		SDL_Rect rect;
		Uint8 alpha;
		double angle;
		int zorder;
		SDL_RendererFlip flip;
		Uint8 r, g, b; 

		bool is_gif;
		int frame_number;
		IMG_Animation* gif;

		SDL_Texture* texture;

	protected:
		

	private:
		
};

