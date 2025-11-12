#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <memory>

#include "SDL/texture.h"
#include "SDL/animation.h"

class Image
{
	public:
		Image(const std::string path, const int x, const int y, SDL_Renderer* renderer, const int zorder=0);

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
		std::unique_ptr<sdl::Animation> gif;

		std::unique_ptr<sdl::Texture> texture; 
		SDL_Renderer* renderer; //solution temporaire => cette classe ne possède pas / don't owns ce pointeur
};