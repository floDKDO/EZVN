#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

namespace sdl
{

class Font
{
	public:
		Font(const std::string_view file, const int ptsize); //TTF_OpenFont
		Font(const Font& font) = delete;
		Font(Font&& font);
		Font& operator=(const Font& font) = delete;
		Font& operator=(Font&& font);
		~Font(); //TTF_CloseFont

		TTF_Font* fetch() const;
		void size_UTF8(const std::string_view text, int* w, int* h) const;
		void set_style(const int style) const;
		void set_outline(const int outline) const;

	private:
		TTF_Font* font_;
};
}
