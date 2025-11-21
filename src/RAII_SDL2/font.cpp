#include "RAII_SDL2/font.h"

#include <iostream>

namespace sdl
{

Font::Font(const std::string file, int ptsize) //TTF_OpenFont
{
	font_ = TTF_OpenFont(file.c_str(), ptsize);
}

Font::~Font() //TTF_CloseFont
{
	TTF_CloseFont(font_);
}

TTF_Font* Font::Get() const
{
	return font_;
}

void Font::size_UTF8(const std::string text, int* w, int* h)
{
	TTF_SizeUTF8(font_, text.c_str(), w, h);
}

void Font::set_style(int style)
{
	TTF_SetFontStyle(this->Get(), style);
}

}