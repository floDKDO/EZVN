#include "color.h"

#include <iostream>

std::unordered_map<std::string, Color> Color::colors_ = 
{
    {"black", Color::from_rgba8(0, 0, 0)},
    {"white",       Color::from_rgba8(255, 255, 255)},
    {"grey",        Color::from_rgba8(128, 128, 128)},
    {"light grey",  Color::from_rgba8(192, 192, 192)},
    {"dark grey",   Color::from_rgba8(64, 64, 64)},
    {"red",         Color::from_rgba8(255, 0, 0)},
    {"dark red",    Color::from_rgba8(127, 0, 0)},
    {"green",       Color::from_rgba8(0, 255, 0)},
    {"lime green",  Color::from_rgba8(182, 255, 0)},
    {"dark green",  Color::from_rgba8(0, 127, 0)},
    {"blue",        Color::from_rgba8(0, 0, 255)},
    {"sky blue",    Color::from_rgba8(149, 200, 216)},
    {"dark blue",   Color::from_rgba8(0, 0, 127)},
    {"yellow",      Color::from_rgba8(255, 255, 0)},
    {"orange",      Color::from_rgba8(255, 106, 0)},
    {"pink",        Color::from_rgba8(255, 0, 255)},
    {"violet",      Color::from_rgba8(129, 0, 255)},
    {"purple",      Color::from_rgba8(90, 0, 255)},
    {"brown",       Color::from_rgba8(124, 71, 0)},
    {"light brown", Color::from_rgba8(153, 121, 80)},
    {"dark brown",  Color::from_rgba8(43, 23, 0)}
};

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
    HSVA hsva_color = {h, s, v, a};
    RGBA8 rgba8_color = hsva_to_rgba8(hsva_color);
    return Color(rgba8_color.r_, rgba8_color.g_, rgba8_color.b_, rgba8_color.a_);
}

Color Color::from_hsva8(double h, Uint8 s, Uint8 v, Uint8 a)
{
    HSVA8 hsva8_color = {h, s, v, a};
    RGBA8 rgba8_color = hsva8_to_rgba8(hsva8_color);
    return Color(rgba8_color.r_, rgba8_color.g_, rgba8_color.b_, rgba8_color.a_);
}

Color Color::from_hex(std::string_view hex_color)
{
    RGBA8 rgba8_color = hex_to_rgba8(hex_color);
    return Color(rgba8_color.r_, rgba8_color.g_, rgba8_color.b_, rgba8_color.a_);
}

Color Color::from_string(std::string_view string_color)
{
    std::string string_color_lower = std::string(string_color);
    for(char& c : string_color_lower)
    {
        c = char(std::tolower(c));
    }

    if(colors_.count(string_color_lower))
    {
        return colors_.at(string_color_lower);
    }
    else
    {
        std::cerr << "Invalid format! (" << string_color << ")\n";
        return Color(0, 0, 0, 255);
    }
}

void Color::add_new_string_color(std::string string_color, Color color)
{
    if(!colors_.count(string_color))
    {
        colors_.insert({string_color, color});
    }
}

SDL_Color Color::to_SDL_Color()
{
    SDL_Color c = {r_, g_, b_, a_};
	return c;
}

Color::RGBA8 Color::hex_to_rgba8(std::string_view hex_color)
{
    std::string hex_values = std::string(hex_color);
    if(hex_color[0] == '#')
    {
        hex_values.erase(0, 1);
    }
    else
    {
        std::cerr << "Invalid format! (" << hex_color << ")\n";
        return {0, 0, 0, 255};
    }

    RGBA8 rgba8_color = {0};
    size_t i = 0;
    for(; i < hex_values.length(); i += 2)
    {
        std::string hex_value = hex_values.substr(i, 2);
        Uint8 rgba_value = static_cast<Uint8>(std::stol(hex_value, nullptr, 16));
        if(i == 0)
        {
            rgba8_color.r_ = rgba_value;
        }
        else if(i == 2)
        {
            rgba8_color.g_ = rgba_value;
        }
        else if(i == 4)
        {
            rgba8_color.b_ = rgba_value;
        }
        else if(i == 6)
        {
            rgba8_color.a_ = rgba_value;
        }
    }

    if(i == 6)
    {
        rgba8_color.a_ = 255; //alpha par défaut si non précisé
    }
    return rgba8_color;
}

//Color::HSVA Color::rgba_to_hsva(RGBA rgba_color) //not used for now
//{
//    double min = std::min(rgba_color.r_, std::min(rgba_color.g_, rgba_color.b_));
//    double max = std::max(rgba_color.r_, std::max(rgba_color.g_, rgba_color.b_));
//    double delta = max - min;
//    double epsilon = 0.00001;
//
//    HSVA hsva_color;
//    hsva_color.v_ = max;                                // v
//    hsva_color.a_ = rgba_color.a_;
//
//    if(delta < epsilon)
//    {
//        hsva_color.s_ = 0;
//        hsva_color.h_ = 0; // undefined, maybe nan?
//        return hsva_color;
//    }
//
//    if(max > 0.0)
//    { // NOTE: if Max is == 0, this divide would cause a crash
//        hsva_color.s_ = (delta / max);                  // s
//    }
//    else
//    {
//        // if max is 0, then r = g = b = 0
//        // s = 0, h is undefined
//        hsva_color.s_ = 0.0;
//        hsva_color.h_ = NAN;                            // its now undefined
//        return hsva_color;
//    }
//
//    if(rgba_color.r_ >= max)                           // ">" is bogus, just keeps compilor happy
//    {
//        hsva_color.h_ = (rgba_color.g_ - rgba_color.b_) / delta;        // between yellow & magenta
//    }
//    else
//    {
//        if(rgba_color.g_ >= max)
//        {
//            hsva_color.h_ = 2.0 + (rgba_color.b_ - rgba_color.r_) / delta;  // between cyan & yellow
//        }
//        else
//        {
//            hsva_color.h_ = 4.0 + (rgba_color.r_ - rgba_color.g_) / delta;  // between magenta & cyan
//        }
//    }
//
//    hsva_color.h_ *= 60.0;                              // degrees
//
//    if(hsva_color.h_ < 0.0)
//    {
//        hsva_color.h_ += 360.0;
//    }
//    return hsva_color;
//}
//
//Color::HSVA Color::rgba8_to_hsva(RGBA8 rgba8_color) //not used for now
//{
//    return rgba_to_hsva({rgba8_color.r_ / 255.0, rgba8_color.g_ / 255.0, rgba8_color.b_ / 255.0, rgba8_color.a_ / 255.0});
//}

Color::RGBA Color::hsva_to_rgba(HSVA hsva_color)
{
    double hh, p, q, t, ff;
    long i;
    Color::RGBA rgba_color;

    if(hsva_color.s_ <= 0.0)
    {       
        rgba_color = {hsva_color.v_, hsva_color.v_, hsva_color.v_, hsva_color.a_};
        return rgba_color;
    }

    hh = hsva_color.h_;

    if(hh >= 360.0)
    {
        hh = 0.0;
    }

    hh /= 60.0;
    i = long(hh);
    ff = hh - i;
    p = hsva_color.v_ * (1.0 - hsva_color.s_);
    q = hsva_color.v_ * (1.0 - (hsva_color.s_ * ff));
    t = hsva_color.v_ * (1.0 - (hsva_color.s_ * (1.0 - ff)));

    switch(i)
    {
        case 0:
            rgba_color = {hsva_color.v_, t, p, hsva_color.a_};
            break;

        case 1:
            rgba_color = {q, hsva_color.v_, p, hsva_color.a_};
            break;

        case 2:
            rgba_color = {p, hsva_color.v_, t, hsva_color.a_};
            break;

        case 3:
            rgba_color = {p, q, hsva_color.v_, hsva_color.a_};
            break;

        case 4:
            rgba_color = {t, p, hsva_color.v_, hsva_color.a_};
            break;

        case 5:
        default:
            rgba_color = {hsva_color.v_, p, q, hsva_color.a_};
            break;
    }
    return rgba_color;
}

Color::RGBA Color::hsva8_to_rgba(HSVA8 hsva8_color)
{
    return hsva_to_rgba({hsva8_color.h_, hsva8_color.s_ / 100.0, hsva8_color.v_ / 100.0, hsva8_color.a_ / 255.0});
}

Color::RGBA8 Color::hsva_to_rgba8(HSVA hsva_color)
{
    RGBA rgba_color = hsva_to_rgba(hsva_color);
    return rgba_to_rgba8(rgba_color);
}

Color::RGBA8 Color::hsva8_to_rgba8(HSVA8 hsva8_color)
{
    RGBA rgba_color = hsva8_to_rgba(hsva8_color);
    return rgba_to_rgba8(rgba_color);
}

Color::RGBA Color::rgba8_to_rgba(RGBA8 rgba8_color)
{
    return {rgba8_color.r_ / 255.0, rgba8_color.g_ / 255.0, rgba8_color.b_ / 255.0, rgba8_color.a_ / 255.0};
}

Color::RGBA8 Color::rgba_to_rgba8(RGBA rgba_color)
{
    return {Uint8(std::round(rgba_color.r_ * 255.0)), Uint8(std::round(rgba_color.g_ * 255.0)), Uint8(std::round(rgba_color.b_ * 255.0)), Uint8(std::round(rgba_color.a_ * 255.0))};
}

Color::HSVA Color::hsva8_to_hsva(HSVA8 hsva8_color)
{
    return {hsva8_color.h_, hsva8_color.s_ / 100.0, hsva8_color.v_ / 100.0, hsva8_color.a_ / 255.0};
}

Color::HSVA8 Color::hsva_to_hsva8(HSVA hsva_color)
{
    return {hsva_color.h_, Uint8(std::round(hsva_color.s_ * 100.0)), Uint8(std::round(hsva_color.v_ * 100.0)), Uint8(std::round(hsva_color.a_ * 255.0))};
}