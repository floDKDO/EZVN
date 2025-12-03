#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

namespace sdl
{

class Font
{
	public:
		Font(const std::string file, const int ptsize); //TTF_OpenFont
		Font(const Font& font) = delete;
		Font& operator=(const Font& font) = delete;
		~Font(); //TTF_CloseFont

		TTF_Font* fetch() const;
		void size_UTF8(const std::string text, int* w, int* h) const;
		void set_style(const int style) const;
		void set_outline(const int outline) const;

	protected:

	private:
		TTF_Font* font_;
};
}
