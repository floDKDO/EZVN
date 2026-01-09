#pragma once

#include <unordered_map>
#include <string>
#include <string_view>
#include <SDL2/SDL.h>

class Color
{
    public: 
        struct RGBA
        {
            double r_, g_, b_, a_; //between 0.0 and 1.0 
        };

        struct RGBA8
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

        static RGBA8 hex_to_rgba8(std::string_view hex_color);
        //static HSVA rgba_to_hsva(RGBA rgba_color); //not used for now
        //static HSVA rgba8_to_hsva(RGBA8 rgba8_color); //not used for now
        static RGBA hsva_to_rgba(HSVA hsva_color);
        static RGBA hsva8_to_rgba(HSVA8 hsva8_color);
        static RGBA8 hsva_to_rgba8(HSVA hsva_color);
        static RGBA8 hsva8_to_rgba8(HSVA8 hsva8_color);

        static RGBA rgba8_to_rgba(RGBA8 rgba8_color);
        static RGBA8 rgba_to_rgba8(RGBA rgba_color);
        static HSVA hsva8_to_hsva(HSVA8 hsva8_color);
        static HSVA8 hsva_to_hsva8(HSVA hsva_color);
};

const std::unordered_map<std::string, Color::RGBA8> colors_ =
{
    {"black",       Color::RGBA8{0, 0, 0, 255}},
    {"white",       Color::RGBA8{255, 255, 255, 255}},
    {"grey",        Color::RGBA8{128, 128, 128, 255}},
    {"light grey",  Color::RGBA8{192, 192, 192, 255}},
    {"dark grey",   Color::RGBA8{64, 64, 64, 255}},
    {"red",         Color::RGBA8{255, 0, 0, 255}},
    {"dark red",    Color::RGBA8{127, 0, 0, 255}},
    {"green",       Color::RGBA8{0, 255, 0, 255}},
    {"lime green",  Color::RGBA8{182, 255, 0, 255}},
    {"dark green",  Color::RGBA8{0, 127, 0, 255}},
    {"blue",        Color::RGBA8{0, 0, 255, 255}},
    {"sky blue",    Color::RGBA8{149, 200, 216, 255}},
    {"dark blue",   Color::RGBA8{0, 0, 127, 255}},
    {"yellow",      Color::RGBA8{255, 255, 0, 255}},
    {"orange",      Color::RGBA8{255, 106, 0, 255}},
    {"pink",        Color::RGBA8{255, 0, 255, 255}},
    {"violet",      Color::RGBA8{129, 0, 255, 255}},
    {"purple",      Color::RGBA8{72, 0, 0, 255}},
    {"brown",       Color::RGBA8{124, 71, 0, 255}},
    {"light brown", Color::RGBA8{153, 121, 80, 255}},
    {"dark brown",  Color::RGBA8{43, 23, 0, 255}}
};
