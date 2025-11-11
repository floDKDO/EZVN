#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

class Text
{
	public:
		Text(const std::string text, const SDL_Color color, const std::string font_path, const int font_size, const int x, const int y, SDL_Renderer* renderer, Uint32 wrap_length=0);
		~Text();
		Text(const Text& t);
		Text& operator=(Text t);

		friend void swap(Text& t1, Text& t2)
		{
			using std::swap;

			swap(t1.text, t2.text);
			swap(t1.color, t2.color);
			swap(t1.font_size, t2.font_size);
			swap(t1.position, t2.position);
			swap(t1.renderer, t2.renderer);
			swap(t1.wrap_length, t2.wrap_length);
			swap(t1.is_dialogue, t2.is_dialogue);
			swap(t1.text_dialogue, t2.text_dialogue);
			swap(t1.index_dialogue, t2.index_dialogue);
			swap(t1.previous_text, t2.previous_text);
			swap(t1.font_path, t2.font_path);
			swap(t1.surface, t2.surface);
			swap(t1.texture, t2.texture);
			swap(t1.font, t2.font);
		}

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

		std::string font_path;

	protected:

	private:
		SDL_Surface* surface;
		SDL_Texture* texture; 
		TTF_Font* font;
};

