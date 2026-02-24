#pragma once

#include "RAII_SDL2/surface.h"
#include "RAII_SDL2/font.h"
#include "RAII_SDL2/renderer.h"
#include "displayable.h"

#include <string>

class Text : public Displayable
{
	public:
		Text(std::string_view text, Color color, std::string_view font_path, int font_size, int x, int y, sdl::Renderer& renderer);
		Text(std::string_view text, Color color, std::string_view font_path, int font_size, int x, int y, sdl::Renderer& renderer, bool is_animated, Uint32 wrap_length);

		void set_italic();
		void set_bold();
		void set_underline();
		void set_strike_through();
		void unset_italic();
		void unset_bold();
		void unset_underline();
		void unset_strike_through();
		void unset_all();

		void change_color(Color color) override;

		void show_outline();
		void hide_outline();

		int get_width_one_char(char c) const;
		int get_width_text() const;
		int get_height_one_char(char c) const; 
		int get_height_text() const; //TODO : attention : ne marche pas si le texte est wrapped !

		void draw(sdl::Renderer& renderer) override;
		void update() override;

		static int global_text_divisor_;
		static const int initial_text_speed_;

		std::string text_;
		std::string text_dialogue_;
		size_t index_dialogue_;
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
		bool outline_visible_;
		
		std::string previous_text_;
		//bool is_animated_; //TODO : préciser une wrap_length mais que le texte ne soit pas animé (pour pop-up et skip des dialogues) ??

		sdl::Surface surface_;
		sdl::Surface surface_outline_;
};

