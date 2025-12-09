#include "RAII_SDL2/font.h"

#include <iostream>

namespace sdl
{

Font::Font(const std::string_view file, const int ptsize) //TTF_OpenFont
{
	if((font_ = TTF_OpenFont(file.data(), ptsize)) == nullptr)
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

void Font::size_UTF8(const std::string_view text, int* w, int* h) const
{
	if(TTF_SizeUTF8(font_, text.data(), w, h) == -1)
	{
		SDL_Log("(TTF_SizeUTF8) %s\n", TTF_GetError());
	}
}

void Font::set_style(const int style) const
{
	TTF_SetFontStyle(font_, style);
}

void Font::set_outline(const int outline) const
{
	TTF_SetFontOutline(font_, outline);
}

}