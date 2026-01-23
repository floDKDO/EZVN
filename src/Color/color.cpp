#include "Color/color.h"
#include "Color/conversion.h"
#include "Color/str.h"

#include <iostream>

using namespace color; //for "conv" and "str"

Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    : r_(r), g_(g), b_(b), a_(a)
{}

Color Color::from_rgba(double r, double g, double b, double a)
{
    return Color(Uint8(std::round(r * 255.0)), Uint8(std::round(g * 255.0)), Uint8(std::round(b * 255.0)), Uint8(std::round(a * 255.0)));
}

Color Color::from_rgba8(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return Color(r, g, b, a);
}

Color Color::from_hsva(double h, double s, double v, double a)
{
    conv::HSVA hsva_color = {h, s, v, a};
    conv::RGBA8 rgba8_color = conv::hsva_to_rgba8(hsva_color);
    return Color(rgba8_color.r_, rgba8_color.g_, rgba8_color.b_, rgba8_color.a_);
}

Color Color::from_hsva8(double h, Uint8 s, Uint8 v, Uint8 a)
{
    conv::HSVA8 hsva8_color = {h, s, v, a};
    conv::RGBA8 rgba8_color = conv::hsva8_to_rgba8(hsva8_color);
    return Color(rgba8_color.r_, rgba8_color.g_, rgba8_color.b_, rgba8_color.a_);
}

Color Color::from_hex(std::string_view hex_color)
{
    conv::RGBA8 rgba8_color = conv::hex_to_rgba8(hex_color);
    return Color(rgba8_color.r_, rgba8_color.g_, rgba8_color.b_, rgba8_color.a_);
}

Color Color::from_string(std::string_view string_color)
{
    std::string string_color_lower = std::string(string_color);
    for(char& c : string_color_lower)
    {
        c = char(std::tolower(c));
    }

    if(str::colors_.count(string_color_lower))
    {
        return str::colors_.at(string_color_lower);
    }
    else
    {
        std::cerr << "Invalid format! (" << string_color << ")\n";
        return Color(0, 0, 0, 255);
    }
}

SDL_Color Color::to_SDL_Color() const
{
    SDL_Color c = {r_, g_, b_, a_};
	return c;
}