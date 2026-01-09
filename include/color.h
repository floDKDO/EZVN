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

//TODO : pouvoir ajouter des entrées dedans
//TODO : fonctionne uniquement si les couleurs sont RGBA8...
const std::unordered_map<std::string, Color> colors_ =
{
    {"black",       Color::from_rgba8(0, 0, 0)},
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
    {"purple",      Color::from_rgba8(72, 0, 0)},
    {"brown",       Color::from_rgba8(124, 71, 0)},
    {"light brown", Color::from_rgba8(153, 121, 80)},
    {"dark brown",  Color::from_rgba8(43, 23, 0)}
};
