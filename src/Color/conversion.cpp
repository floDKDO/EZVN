#include "Color/conversion.h"

#include <string>
#include <iostream>

namespace color::conv
{

RGBA8 hex_to_rgba8(std::string_view hex_color)
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

HSVA rgba_to_hsva(RGBA rgba_color) //not used for now
{
    double min = std::min(rgba_color.r_, std::min(rgba_color.g_, rgba_color.b_));
    double max = std::max(rgba_color.r_, std::max(rgba_color.g_, rgba_color.b_));
    double delta = max - min;
    double epsilon = 0.00001;

    HSVA hsva_color;
    hsva_color.v_ = max;                                // v
    hsva_color.a_ = rgba_color.a_;

    if(delta < epsilon)
    {
        hsva_color.s_ = 0;
        hsva_color.h_ = 0; // undefined, maybe nan?
        return hsva_color;
    }

    if(max > 0.0)
    { // NOTE: if Max is == 0, this divide would cause a crash
        hsva_color.s_ = (delta / max);                  // s
    }
    else
    {
        // if max is 0, then r = g = b = 0
        // s = 0, h is undefined
        hsva_color.s_ = 0.0;
        hsva_color.h_ = NAN;                            // its now undefined
        return hsva_color;
    }

    if(rgba_color.r_ >= max)                           // ">" is bogus, just keeps compilor happy
    {
        hsva_color.h_ = (rgba_color.g_ - rgba_color.b_) / delta;        // between yellow & magenta
    }
    else
    {
        if(rgba_color.g_ >= max)
        {
            hsva_color.h_ = 2.0 + (rgba_color.b_ - rgba_color.r_) / delta;  // between cyan & yellow
        }
        else
        {
            hsva_color.h_ = 4.0 + (rgba_color.r_ - rgba_color.g_) / delta;  // between magenta & cyan
        }
    }

    hsva_color.h_ *= 60.0;                              // degrees

    if(hsva_color.h_ < 0.0)
    {
        hsva_color.h_ += 360.0;
    }
    return hsva_color;
}

HSVA rgba8_to_hsva(RGBA8 rgba8_color) //not used for now
{
    return rgba_to_hsva({rgba8_color.r_ / 255.0, rgba8_color.g_ / 255.0, rgba8_color.b_ / 255.0, rgba8_color.a_ / 255.0});
}

RGBA hsva_to_rgba(HSVA hsva_color)
{
    double hh, p, q, t, ff;
    long i;
    RGBA rgba_color;

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

RGBA hsva8_to_rgba(HSVA8 hsva8_color)
{
    return hsva_to_rgba({hsva8_color.h_, hsva8_color.s_ / 100.0, hsva8_color.v_ / 100.0, hsva8_color.a_ / 255.0});
}

RGBA8 hsva_to_rgba8(HSVA hsva_color)
{
    RGBA rgba_color = hsva_to_rgba(hsva_color);
    return rgba_to_rgba8(rgba_color);
}

RGBA8 hsva8_to_rgba8(HSVA8 hsva8_color)
{
    RGBA rgba_color = hsva8_to_rgba(hsva8_color);
    return rgba_to_rgba8(rgba_color);
}

RGBA rgba8_to_rgba(RGBA8 rgba8_color)
{
    return {rgba8_color.r_ / 255.0, rgba8_color.g_ / 255.0, rgba8_color.b_ / 255.0, rgba8_color.a_ / 255.0};
}

RGBA8 rgba_to_rgba8(RGBA rgba_color)
{
    return {Uint8(std::round(rgba_color.r_ * 255.0)), Uint8(std::round(rgba_color.g_ * 255.0)), Uint8(std::round(rgba_color.b_ * 255.0)), Uint8(std::round(rgba_color.a_ * 255.0))};
}

HSVA hsva8_to_hsva(HSVA8 hsva8_color)
{
    return {hsva8_color.h_, hsva8_color.s_ / 100.0, hsva8_color.v_ / 100.0, hsva8_color.a_ / 255.0};
}

HSVA8 hsva_to_hsva8(HSVA hsva_color)
{
    return {hsva_color.h_, Uint8(std::round(hsva_color.s_ * 100.0)), Uint8(std::round(hsva_color.v_ * 100.0)), Uint8(std::round(hsva_color.a_ * 255.0))};
}

}