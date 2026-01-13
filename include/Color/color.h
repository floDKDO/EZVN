#pragma once

#include <unordered_map>
#include <string>
#include <string_view>
#include <SDL2/SDL.h>

class Color
{
    public: 
        //Factories
        static Color from_rgba(double r, double g, double b, double a = 1.0);
        static Color from_rgba8(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
        static Color from_hsva(double h, double s, double v, double a = 1.0);
        static Color from_hsva8(double h, Uint8 s, Uint8 v, Uint8 a = 255);
        static Color from_hex(std::string_view hex_color);
        static Color from_string(std::string_view string_color);

        SDL_Color to_SDL_Color();

        Uint8 r_, g_, b_, a_; //stored in RGBA8 format (values between 0 and 255)

    private:
        Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
};
