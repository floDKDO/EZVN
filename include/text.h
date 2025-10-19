#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

class Text
{
	public:
		Text(std::string text, SDL_Color color, std::string font_path, int x, int y, SDL_Renderer* renderer); //TODO : police = argument par défaut
		~Text();

		TTF_Font* font;
		std::string text;
		SDL_Color color;
		SDL_Rect position;

		SDL_Surface* surface;
		SDL_Texture* texture;

		void edit_text(std::string new_text, SDL_Renderer* renderer);
		void draw(SDL_Renderer* renderer);

	protected:

	private:

};

