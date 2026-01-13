#pragma once

#include <SDL2/SDL.h>
#include <string_view>

namespace color::conv
{
    struct RGBA
    {
        double r_, g_, b_, a_; //between 0.0 and 1.0 
    };

    struct RGBA8 //<=> SDL_Color
    {
        Uint8 r_, g_, b_, a_; //between 0 and 255 
    };

    struct HSVA
    {
        double h_; //between 0 and 360 (degrees)
        double s_, v_, a_; //between 0.0 and 1.0 
    };

    struct HSVA8
    {
        double h_; //between 0 and 360 (degrees)
        Uint8 s_, v_; //between 0 and 100 
        Uint8 a_; //between 0 and 255 
    };

    RGBA8 hex_to_rgba8(std::string_view hex_color);
    HSVA rgba_to_hsva(RGBA rgba_color); //not used for now
    HSVA rgba8_to_hsva(RGBA8 rgba8_color); //not used for now
    RGBA hsva_to_rgba(HSVA hsva_color);
    RGBA hsva8_to_rgba(HSVA8 hsva8_color);
    RGBA8 hsva_to_rgba8(HSVA hsva_color);
    RGBA8 hsva8_to_rgba8(HSVA8 hsva8_color);
    RGBA rgba8_to_rgba(RGBA8 rgba8_color);
    RGBA8 rgba_to_rgba8(RGBA rgba_color);
    HSVA hsva8_to_hsva(HSVA8 hsva8_color);
    HSVA8 hsva_to_hsva8(HSVA hsva_color);
}