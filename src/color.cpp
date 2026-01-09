#include "color.h"

#include <iostream>

Color::Color(RGBA rgba_color)
	: r_(rgba_color.r_), g_(rgba_color.g_), b_(rgba_color.b_), a_(rgba_color.a_)
{}

Color::Color(RGBA8 rgba8_color) 
{
    rgba8_to_rgba(rgba8_color);
    //std::cout << "Transformed RGBA8 => " << r_ << ", " << g_ << ", " << b_ << ", " << a_ << std::endl;
}

Color::Color(HSVA hsva_color)
{
    hsva_to_rgba(hsva_color);
    //std::cout << "Transformed HSVA => " << r_ << ", " << g_ << ", " << b_ << ", " << a_ << std::endl;
}

Color::Color(HSVA8 hsva8_color)
{
    hsva8_to_rgba(hsva8_color);
    //std::cout << "Transformed HSVA8 => " << r_ << ", " << g_ << ", " << b_ << ", " << a_ << std::endl;
}

Color::Color(std::string_view color) //string et hex
{
	if(color[0] == '#')
	{
        hex_to_rgba(color);
        //std::cout << "Transformed HEX => " << r_ << ", " << g_ << ", " << b_ << ", " << a_ << std::endl;
	}
	else
	{
        if(colors_.count(std::string(color)))
        {
            const RGBA8 c = colors_.at(std::string(color));
            r_ = c.r_ / 255.0;
            g_ = c.g_ / 255.0;
            b_ = c.b_ / 255.0;
            a_ = c.a_ / 255.0;
            //std::cout << "Transformed STRING => " << r_ << ", " << g_ << ", " << b_ << ", " << a_ << std::endl;
        }
        else
        {
            std::cerr << "Invalid format!\n";
            r_ = 0.0;
            g_ = 0.0;
            b_ = 0.0;
            a_ = 1.0;
        }
	}
}

SDL_Color Color::to_SDL_Color()
{
    SDL_Color c = {Uint8(std::round(r_ * 255.0)), Uint8(std::round(g_ * 255.0)), Uint8(std::round(b_ * 255.0)), Uint8(std::round(a_ * 255.0))};
	return c;
}

void Color::hex_to_rgba(std::string_view hex_color)
{
    std::string hex_values = std::string(hex_color);
    if(hex_color[0] == '#')
    {
        hex_values.erase(0, 1);
    }
    else
    {
        std::cerr << "Invalid format!\n";
        r_ = 0.0;
        g_ = 0.0;
        b_ = 0.0;
        a_ = 1.0;
        return;
    }

    size_t i = 0;
    for(; i < hex_values.length(); i += 2)
    {
        std::string hex_value = hex_values.substr(i, 2);
        Uint8 rgba_value = static_cast<Uint8>(std::stol(hex_value, nullptr, 16));
        if(i == 0)
        {
            r_ = rgba_value / 255.0;
        }
        else if(i == 2)
        {
            g_ = rgba_value / 255.0;
        }
        else if(i == 4)
        {
            b_ = rgba_value / 255.0;
        }
        else if(i == 6)
        {
            a_ = rgba_value / 255.0;
        }
    }

    if(i == 6)
    {
        a_ = 1.0; //alpha par défaut si non précisé
    }
}

//void Color::rgba_to_hsva(RGBA rgba_color) //not used for now
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
//
//        r_ = hsva_color.h_;
//        g_ = hsva_color.s_;
//        b_ = hsva_color.v_;
//        a_ = hsva_color.a_;
//        return;
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
//
//        r_ = hsva_color.h_;
//        g_ = hsva_color.s_;
//        b_ = hsva_color.v_;
//        a_ = hsva_color.a_;
//        return;
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
//
//    r_ = hsva_color.h_;
//    g_ = hsva_color.s_;
//    b_ = hsva_color.v_;
//    a_ = hsva_color.a_;
//
//    //std::cout << "Transformed RGBA => " << r_ << ", " << g_ << ", " << b_ << ", " << a_ << std::endl;
//}

//void Color::rgba8_to_hsva(RGBA8 rgba8_color) //not used for now
//{
//    rgba_to_hsva({rgba8_color.r_ / 255.0, rgba8_color.g_ / 255.0, rgba8_color.b_ / 255.0, rgba8_color.a_ / 255.0});
//}

void Color::hsva_to_rgba(HSVA hsva_color)
{
    double hh, p, q, t, ff;
    long i;
    Color::RGBA rgba_color;

    if(hsva_color.s_ <= 0.0)
    {       
        rgba_color = {hsva_color.v_, hsva_color.v_, hsva_color.v_, hsva_color.a_};
        r_ = rgba_color.r_;
        g_ = rgba_color.g_;
        b_ = rgba_color.b_;
        a_ = rgba_color.a_;
        return;
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
    r_ = rgba_color.r_;
    g_ = rgba_color.g_;
    b_ = rgba_color.b_;
    a_ = rgba_color.a_;
}

void Color::hsva8_to_rgba(HSVA8 hsva8_color)
{
    hsva_to_rgba({hsva8_color.h_, hsva8_color.s_ / 100.0, hsva8_color.v_ / 100.0, hsva8_color.a_ / 255.0});
}

void Color::rgba8_to_rgba(RGBA8 rgba8_color)
{
    r_ = rgba8_color.r_ / 255.0;
    g_ = rgba8_color.g_ / 255.0;
    b_ = rgba8_color.b_ / 255.0;
    a_ = rgba8_color.a_ / 255.0;
}

void Color::rgba_to_rgba8(RGBA rgba_color)
{
    r_ = rgba_color.r_ * 255.0;
    g_ = rgba_color.g_ * 255.0;
    b_ = rgba_color.b_ * 255.0;
    a_ = rgba_color.a_ * 255.0;
}

void Color::hsva8_to_hsva(HSVA8 hsva8_color)
{
    r_ = hsva8_color.h_;
    g_ = hsva8_color.s_ / 100.0;
    b_ = hsva8_color.v_ / 100.0;
    a_ = hsva8_color.a_ / 255.0;
}

void Color::hsva_to_hsva8(HSVA hsva_color)
{
    r_ = hsva_color.h_;
    g_ = hsva_color.s_ * 100.0;
    b_ = hsva_color.v_ * 100.0;
    a_ = hsva_color.a_ * 255.0;
}