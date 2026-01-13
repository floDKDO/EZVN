#include "Color/str.h"

namespace color::str
{

std::unordered_map<std::string, Color> colors_ =
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
    {"purple",      Color::from_rgba8(90, 0, 255)},
    {"brown",       Color::from_rgba8(124, 71, 0)},
    {"light brown", Color::from_rgba8(153, 121, 80)},
    {"dark brown",  Color::from_rgba8(43, 23, 0)}
};

void add_new_string_color(std::string string_color, Color color)
{
    if(!colors_.count(string_color))
    {
        colors_.insert({string_color, color});
    }
}

}