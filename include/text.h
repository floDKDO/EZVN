#pragma once

#include "RAII_SDL2/surface.h"
#include "RAII_SDL2/texture.h"
#include "RAII_SDL2/font.h"
#include "RAII_SDL2/renderer.h"

#include <memory>

class Text
{
	public:
		Text(const std::string text, const SDL_Color color, const std::string font_path, const int font_size, const int x, const int y, sdl::Renderer& renderer, const Uint32 wrap_length=0);

		void show();
		void hide();
		void change_color(const SDL_Color color);

		void set_italic();
		void set_bold();
		void set_underline();
		void set_strike_through();
		void unset_italic();
		void unset_bold();
		void unset_underline();
		void unset_strike_through();
		void unset_all();

		int get_width_one_char(const char c) const;
		int get_width_text() const;
		int get_height_one_char(const char c) const;
		int get_height_text() const;

		void draw(sdl::Renderer& renderer);
		void update();

		static int global_text_divisor_;

	private:
		void create_surface_texture();

		std::unique_ptr<sdl::Surface> surface_;
		std::unique_ptr<sdl::Surface> surface_outline_;
		std::unique_ptr<sdl::Texture> texture_;

	public:
		std::string text_;
		std::string text_dialogue_;
		int index_dialogue_;
		bool is_finished_;

		//int local_text_speed_; //TODO : unité ?? (actuellement, agrandir cette valeur réduit la vitesse d'affichage) => objectif = cps

		Uint64 last_time_; 

		Uint32 wrap_length_;

		SDL_Rect position_;

	protected:

	private:
		SDL_Color color_;

		int font_size_;
		int font_style_;
		int previous_font_style_;
		std::string font_path_;
		sdl::Font font_;
		sdl::Font font_outline_;
		int outline_size_;

		bool is_dialogue_;

		std::string previous_text_;
		
		sdl::Renderer& renderer_; //solution temporaire

		static const int initial_text_speed_;
};

