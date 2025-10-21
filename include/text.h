#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

class Text
{
	public:
		Text(const std::string text, const SDL_Color color, const std::string font_path, const int font_size, const int x, const int y, SDL_Renderer* renderer); 
		~Text();

		void show();
		void hide();
		void change_color(SDL_Color color);
		void draw(SDL_Renderer* renderer);
		void update(Uint64& time_step);

		std::string text;
		SDL_Color color;
		int font_size;
		SDL_Rect position;
		SDL_Renderer* renderer; //solution temporaire

	protected:

	private:
		TTF_Font* font;
		SDL_Surface* surface;
		SDL_Texture* texture; 
};

