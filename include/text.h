#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

class Text
{
	public:
		Text(const std::string text, const SDL_Color color, const std::string font_path, const int font_size, const int x, const int y, SDL_Renderer* renderer, Uint32 wrap_length=0);
		~Text();

		void show();
		void hide();
		void change_color(SDL_Color color);
		int get_width_one_char(char c);
		void draw(SDL_Renderer* renderer);
		void update(Uint64& time_step);

		std::string text;
		SDL_Color color;
		int font_size;
		SDL_Rect position;
		SDL_Renderer* renderer; //solution temporaire
		Uint32 wrap_length;

		bool is_dialogue;
		std::string text_dialogue;
		int index_dialogue;

		std::string previous_text;

	protected:

	private:
		SDL_Surface* surface;
		SDL_Texture* texture; 
		TTF_Font* font;
};

