#include "RAII_SDL2/font.h"

#include <iostream>

namespace sdl
{

Font::Font(const std::string file, int ptsize) //TTF_OpenFont
{
	if((font_ = TTF_OpenFont(file.c_str(), ptsize)) == nullptr)
	{
		SDL_Log("(TTF_OpenFont) %s\n", TTF_GetError());
	}
}

Font::~Font() //TTF_CloseFont
{
	TTF_CloseFont(font_);
}

TTF_Font* Font::fetch() const
{
	return font_;
}

void Font::size_UTF8(const std::string text, int* w, int* h)
{
	if(TTF_SizeUTF8(font_, text.c_str(), w, h) == -1)
	{
		SDL_Log("(TTF_SizeUTF8) %s\n", TTF_GetError());
	}
}

void Font::set_style(int style)
{
	TTF_SetFontStyle(font_, style);
}

void Font::set_outline(int outline)
{
	TTF_SetFontOutline(font_, outline);
}

}