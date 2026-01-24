#pragma once

#include "Color/color.h"

#include <unordered_map>
#include <string>

namespace color::str
{

extern std::unordered_map<std::string, Color> colors_;

void add_new_string_color(std::string string_color, Color color);

}