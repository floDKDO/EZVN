#include "RAII_SDL2/font.h"

#include <iostream>

namespace sdl
{

Font::Font(const std::string file, int ptsize) //TTF_OpenFont
{
	this->font = TTF_OpenFont(file.c_str(), ptsize);
}

Font::~Font() //TTF_CloseFont
{
	TTF_CloseFont(this->font);
}

TTF_Font* Font::Get() const
{
	return this->font;
}

void Font::size_UTF8(const std::string text, int* w, int* h)
{
	TTF_SizeUTF8(this->font, text.c_str(), w, h);
}

}