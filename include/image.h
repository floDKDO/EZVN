#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

class Image
{
	public:
		Image(std::string path, SDL_Renderer* renderer);
		~Image();
		void show(SDL_Renderer* renderer);
		void hide(SDL_Renderer* renderer);
		void flip_vertically();
		void flip_horizontally();
		void flip_normal();
		void resize(int w, int h);
		void position(int x, int y);

		SDL_Rect rect;
		int alpha;
		double angle;
		int zorder;
		SDL_RendererFlip flip;

		SDL_Texture* texture;

	protected:
		

	private:
		
};

