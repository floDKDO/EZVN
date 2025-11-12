#include "SDL/font.h"
#include <iostream>
namespace sdl
{

Font::Font(const std::string file, int ptsize) //TTF_OpenFont
{
	this->font = TTF_OpenFont(file.c_str(), ptsize);
}

Font::Font(const Font& font)
{
	//TODO!
}

Font& Font::operator=(const Font& font)
{
	//TODO!
	return *this;
}


Font::~Font() //TTF_CloseFont
{
	std::cout << "DESTRUECTEUR DE FONT!\n";
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