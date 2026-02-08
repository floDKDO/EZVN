#pragma once

#include <string>
#include <string_view>

namespace utils
{

template<typename Arithmetic1, typename Arithmetic2, typename Arithmetic3>
double lerp(Arithmetic1 a, Arithmetic2 b, Arithmetic3 t) //TODO : à terme, remplacer par la fonction std::lerp de C++20
{
	double a_d = static_cast<double>(a);
	double b_d = static_cast<double>(b);
	double t_d = static_cast<double>(t);

	return a_d + t_d * (b_d - a_d);
}

std::string string_to_lower(std::string_view string);

}