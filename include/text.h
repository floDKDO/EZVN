#pragma once

#include "RAII_SDL2/surface.h"
#include "RAII_SDL2/texture.h"
#include "RAII_SDL2/font.h"
#include "RAII_SDL2/renderer.h"
#include "drawable.h"

#include <memory>

class Text : public Drawable
{
	public:
		Text(const std::string_view text, const SDL_Color color, const std::string_view font_path, const int font_size, const int x, const int y, sdl::Renderer& renderer, const Uint32 wrap_length=0);

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

		void draw(sdl::Renderer& renderer) override;
		void update();

		static int global_text_divisor_;

	private:
		void create_surface_texture();

		static const int initial_text_speed_;

		std::unique_ptr<sdl::Surface> surface_;
		std::unique_ptr<sdl::Surface> surface_outline_;

	public:
		std::string text_;
		std::string text_dialogue_;
		int index_dialogue_;
		bool is_finished_;

		//int local_text_speed_; //TODO : unité ?? (actuellement, agrandir cette valeur réduit la vitesse d'affichage) => objectif = cps

		Uint32 wrap_length_;

	private:
		int font_size_;
		int font_style_;
		int previous_font_style_;
		std::string_view font_path_;
		sdl::Font font_;
		sdl::Font font_outline_;
		int outline_size_;

		bool is_dialogue_;

		std::string previous_text_;
		
		sdl::Renderer& renderer_; //solution temporaire
};

