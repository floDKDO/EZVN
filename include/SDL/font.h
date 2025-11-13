#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

namespace sdl
{
	class Font;
}

class sdl::Font
{
	public:
		Font(const std::string file, int ptsize); //TTF_OpenFont
		Font(const Font& font) = delete;
		Font& operator=(const Font& font) = delete;
		~Font(); //TTF_CloseFont

		TTF_Font* Get() const;
		void size_UTF8(const std::string text, int* w, int* h);

	protected:

	private:
		TTF_Font* font;
};

