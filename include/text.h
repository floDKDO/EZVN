#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

class Text
{
	public:
		Text(const std::string text, const SDL_Color color, const std::string font_path, int font_size, const int x, const int y, SDL_Renderer* renderer); //TODO : police = argument par défaut
		~Text();

		TTF_Font* font;
		int font_size;
		std::string text;
		SDL_Color color;
		SDL_Rect position;

		SDL_Surface* surface;
		SDL_Texture* texture;

		void edit_text(SDL_Renderer* renderer);
		void draw(SDL_Renderer* renderer);

	protected:

	private:

};

