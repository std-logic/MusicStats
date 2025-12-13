#pragma once

#include "BasicTypes.h"

namespace Helper
{
	inline auto checkMinYear(unsigned int curr_min_year, unsigned int year)
	{
		if (year != BasicTypes::undefined_year) {
			if ((curr_min_year > year) || (curr_min_year == BasicTypes::undefined_year)) {
				curr_min_year = year;
			}
		}
		return curr_min_year;
	}
	inline auto checkMaxYear(unsigned int curr_max_year, unsigned int year)
	{
		if (year != BasicTypes::undefined_year) {
			if ((curr_max_year < year) || (curr_max_year == BasicTypes::undefined_year)) {
				curr_max_year = year;
			}
		}
		return curr_max_year;
	}
	inline BasicTypes::StringType yearString(unsigned int min_year, unsigned int max_year)
	{
		return	(min_year == BasicTypes::undefined_year) ?
				BasicTypes::StringType("") : (min_year == max_year) ?
				std::to_string(min_year) :
				std::to_string(min_year) + "-" + std::to_string(max_year);
	}
}
