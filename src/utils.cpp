#include "utils.h"

namespace utils
{

std::string string_to_lower(std::string_view string)
{
	std::string string_lower = std::string(string);
	for(char& c : string_lower)
	{
		c = char(std::tolower(c));
	}
	return string_lower;
}

}