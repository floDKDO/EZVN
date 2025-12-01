#pragma once

#include "RAII_SDL2/surface.h"
#include "RAII_SDL2/texture.h"
#include "RAII_SDL2/font.h"
#include "RAII_SDL2/renderer.h"

#include <memory>

class Text
{
	public:
		Text(const std::string text, const SDL_Color color, const std::string font_path, const int font_size, const int x, const int y, sdl::Renderer& renderer, Uint32 wrap_length=0);

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
		int get_height_one_char(char c); 
		int get_height_text();

		void draw(sdl::Renderer& renderer);
		void update();

		std::string text_;
		SDL_Color color_;
		int font_size_;
		int font_style_;
		SDL_Rect position_;
		sdl::Renderer& renderer_; //solution temporaire
		Uint32 wrap_length_;

		bool is_dialogue_;
		std::string text_dialogue_;
		int index_dialogue_;

		static int global_text_divisor_;
		static const int initial_text_speed_ = 500;

		//int local_text_speed_; //TODO : unité ?? (actuellement, agrandir cette valeur réduit la vitesse d'affichage) => objectif = cps

		bool is_finished_;

		std::string previous_text_;
		int previous_font_style_;

		std::string font_path_;

		Uint64 last_time_; 

	protected:

	private:
		void create_surface_texture();

		std::unique_ptr<sdl::Surface> surface_;
		std::unique_ptr<sdl::Surface> surface_outline_;
		std::unique_ptr<sdl::Texture> texture_;
		sdl::Font font_;
		sdl::Font font_outline_;
		int outline_size_;
};

