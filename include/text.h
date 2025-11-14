#pragma once

#include "SDL/surface.h"
#include "SDL/texture.h"
#include "SDL/font.h"

#include <memory>

class Text
{
	public:
		Text(const std::string text, const SDL_Color color, const std::string font_path, const int font_size, const int x, const int y, SDL_Renderer* renderer, Uint32 wrap_length=0);

		void show();
		void hide();
		void change_color(SDL_Color color);
		int get_width_one_char(char c);
		int get_width_text();
		void draw(SDL_Renderer* renderer);
		void update(Uint64 time_step);

		std::string text;
		SDL_Color color;
		int font_size;
		SDL_Rect position;
		SDL_Renderer* renderer; //solution temporaire
		Uint32 wrap_length;

		bool is_dialogue;
		std::string text_dialogue;
		int index_dialogue;
		bool is_finished;

		std::string previous_text;

		std::string font_path;

		Uint64 last_time; //TODO : trouver un meilleur nom

	protected:

	private:
		std::unique_ptr<sdl::Surface> surface;
		std::unique_ptr<sdl::Texture> texture;
		std::unique_ptr<sdl::Font> font;
};

