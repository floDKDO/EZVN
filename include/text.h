#pragma once

#include "RAII_SDL2/surface.h"
#include "RAII_SDL2/texture.h"
#include "RAII_SDL2/font.h"

#include <memory>

class Text
{
	public:
		Text(const std::string text, const SDL_Color color, const std::string font_path, const int font_size, const int x, const int y, SDL_Renderer* renderer, Uint32 wrap_length=0);

		void show();
		void hide();
		void change_color(SDL_Color color);

		void set_italic();
		void set_bold();
		void set_underline();
		void set_strike_through();
		void unset_italic();
		void unset_bold();
		void unset_underline();
		void unset_strike_through();
		void unset_all();

		int get_width_one_char(char c);
		int get_width_text();
		void draw(SDL_Renderer* renderer);
		void update(Uint64 time_step);

		std::string text;
		SDL_Color color;
		int font_size;
		int font_style;
		SDL_Rect position;
		SDL_Renderer* renderer; //solution temporaire
		Uint32 wrap_length;

		bool is_dialogue;
		std::string text_dialogue;
		int index_dialogue;

		//TODO : faire un réglage "opaque" comme Text Speed dans Options de Ren'Py ??
		int text_speed; //TODO : unité ?? (actuellement, agrandir cette valeur réduit la vitesse d'affichage) => objectif = cps

		bool is_finished;

		std::string previous_text;
		int previous_font_style;

		std::string font_path;

		Uint64 last_time; //TODO : trouver un meilleur nom

	protected:

	private:
		void create_surface_texture();

		std::unique_ptr<sdl::Surface> surface;
		std::unique_ptr<sdl::Texture> texture;
		std::unique_ptr<sdl::Font> font;
};

