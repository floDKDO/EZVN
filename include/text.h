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
		Text(const std::string_view text, Color color, const std::string_view font_path, const int font_size, const int x, const int y, sdl::Renderer& renderer);
		Text(const std::string_view text, Color color, const std::string_view font_path, const int font_size, const int x, const int y, sdl::Renderer& renderer, bool is_animated, const Uint32 wrap_length);

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
		static int initial_text_speed_;

		std::string text_;
		std::string text_dialogue_;
		int index_dialogue_;
		bool is_finished_;
		Uint32 wrap_length_;
		//int local_text_speed_; //TODO : unité ?? (actuellement, agrandir cette valeur réduit la vitesse d'affichage) => objectif = cps

		bool is_animated_; //TODO : préciser une wrap_length mais que le texte ne soit pas animé (pour pop-up et skip des dialogues) ??

	private:
		sdl::Font create_outline();

		sdl::Surface create_surface(sdl::Font& font, Color color);
		void recreate_surfaces_texture();

		int font_size_;
		int font_style_;
		int previous_font_style_;
		std::string font_path_;
		sdl::Font font_;

		int outline_size_; 
		sdl::Font font_outline_; //TODO : mettre l'outline en optionnel
		
		std::string previous_text_;
		//bool is_animated_; //TODO : préciser une wrap_length mais que le texte ne soit pas animé (pour pop-up et skip des dialogues) ??

		sdl::Surface surface_;
		sdl::Surface surface_outline_;
};

