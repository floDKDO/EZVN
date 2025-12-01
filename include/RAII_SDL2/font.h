#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

namespace sdl
{

class Font
{
	public:
		Font(const std::string file, int ptsize); //TTF_OpenFont
		Font(const Font& font) = delete;
		Font& operator=(const Font& font) = delete;
		~Font(); //TTF_CloseFont

		TTF_Font* fetch() const;
		void size_UTF8(const std::string text, int* w, int* h);
		void set_style(int style);
		void set_outline(int outline);

	protected:

	private:
		TTF_Font* font_;
};
}
